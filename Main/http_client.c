/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * HTTP Upload Client (Streaming Chunked Version)
 * ============================================================
 */

#include "http_client.h"
#include "wifi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "HTTP";

/*-------------------------------------------------------------
 * Response Buffer
 *------------------------------------------------------------*/
static char response_buffer[HTTP_RESPONSE_BUFFER_SIZE];
static int response_length = 0;

/*-------------------------------------------------------------
 * HTTP Event Handler
 *------------------------------------------------------------*/
static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
        case HTTP_EVENT_ERROR:
            ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
            break;

        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "Connected to server");
            break;

        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "Header sent");
            break;

        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "Header %s : %s", evt->header_key, evt->header_value);
            break;

        case HTTP_EVENT_ON_DATA:
            if (!esp_http_client_is_chunked_response(evt->client))
            {
                if ((response_length + evt->data_len) < HTTP_RESPONSE_BUFFER_SIZE)
                {
                    memcpy(response_buffer + response_length, evt->data, evt->data_len);
                    response_length += evt->data_len;
                    response_buffer[response_length] = '\0';
                }
            }
            break;

        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP transfer complete");
            break;

        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected");
            break;

        default:
            break;
    }
    return ESP_OK;
}

/*-------------------------------------------------------------
 * Check Server
 *------------------------------------------------------------*/
bool http_server_available(void)
{
    esp_http_client_config_t config = {
        .url = TRANSCRIBE_URL,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 3000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL)
    {
        return false;
    }

    esp_err_t err = esp_http_client_perform(client);
    esp_http_client_cleanup(client);

    return (err == ESP_OK);
}

/*-------------------------------------------------------------
 * Upload WAV File via Streaming API
 *------------------------------------------------------------*/
esp_err_t http_upload_wav(char *recognized_text, size_t max_len)
{
    if (recognized_text == NULL || max_len == 0)
    {
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "==================================");
    ESP_LOGI(TAG, "Streaming WAV to Whisper Server...");
    ESP_LOGI(TAG, "==================================");

    FILE *fp = fopen("/spiffs/record.wav", "rb");
    if (fp == NULL)
    {
        ESP_LOGE(TAG, "Cannot open record.wav");
        return ESP_FAIL;
    }

    /*-------------------------------------------------------------
     * Get WAV Size
     *------------------------------------------------------------*/

    fseek(fp, 0, SEEK_END);
    long wav_size = ftell(fp);
    rewind(fp);

    ESP_LOGI(TAG, "WAV Size : %ld bytes", wav_size);

    response_length = 0;
    memset(response_buffer, 0, sizeof(response_buffer));

    /*-------------------------------------------------------------
     * Multipart Form Data
     *------------------------------------------------------------*/

    const char *boundary = "------------------------ESP32P4Boundary";

    char header[256];

    snprintf(header,
             sizeof(header),
             "--%s\r\n"
             "Content-Disposition: form-data; name=\"file\"; filename=\"record.wav\"\r\n"
             "Content-Type: audio/wav\r\n\r\n",
             boundary);

    char footer[128];

    snprintf(footer,
             sizeof(footer),
             "\r\n--%s--\r\n",
             boundary);

    int total_post_size =
        strlen(header) +
        wav_size +
        strlen(footer);

    /*-------------------------------------------------------------
     * HTTP Client
     *------------------------------------------------------------*/

    esp_http_client_config_t config =
    {
        .url = TRANSCRIBE_URL,
        .method = HTTP_METHOD_POST,
        .timeout_ms = 60000,
        .event_handler = http_event_handler,
    };

    esp_http_client_handle_t client =
        esp_http_client_init(&config);

    if (client == NULL)
    {
        fclose(fp);
        ESP_LOGE(TAG, "Cannot create HTTP client");
        return ESP_FAIL;
    }

    char content_type[128];

    snprintf(content_type,
             sizeof(content_type),
             "multipart/form-data; boundary=%s",
             boundary);

    esp_http_client_set_header(
        client,
        "Content-Type",
        content_type);

    esp_err_t err =
        esp_http_client_open(
            client,
            total_post_size);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG,
                 "Failed to open HTTP connection");

        esp_http_client_cleanup(client);
        fclose(fp);
        return err;
    }

    /*-------------------------------------------------------------
     * Send Header
     *------------------------------------------------------------*/

    esp_http_client_write(
        client,
        header,
        strlen(header));

    /*-------------------------------------------------------------
     * Stream WAV
     *------------------------------------------------------------*/

    char *chunk_buffer = malloc(4096);

    if (chunk_buffer == NULL)
    {
        fclose(fp);
        esp_http_client_cleanup(client);
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(TAG, "Streaming audio payload...");

    size_t read_bytes;

    while ((read_bytes =
            fread(chunk_buffer,
                  1,
                  4096,
                  fp)) > 0)
    {
        if (esp_http_client_write(
                client,
                chunk_buffer,
                read_bytes) < 0)
        {
            free(chunk_buffer);
            fclose(fp);
            esp_http_client_cleanup(client);

            ESP_LOGE(TAG,
                     "Write failed");

            return ESP_FAIL;
        }
    }

    free(chunk_buffer);
    fclose(fp);

    /*-------------------------------------------------------------
     * Footer
     *------------------------------------------------------------*/

    esp_http_client_write(
        client,
        footer,
        strlen(footer));

    /*-------------------------------------------------------------
     * Receive Response
     *------------------------------------------------------------*/

    int status =
        esp_http_client_fetch_headers(client);

    if (status < 0)
    {
        ESP_LOGE(TAG,
                 "Fetch headers failed");

        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    memset(response_buffer,
           0,
           sizeof(response_buffer));

    int len =
        esp_http_client_read_response(
            client,
            response_buffer,
            sizeof(response_buffer) - 1);

    if (len < 0)
    {
        ESP_LOGE(TAG,
                 "Failed to read response");

        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    response_buffer[len] = '\0';

    ESP_LOGI(TAG,
             "HTTP Status = %d",
             esp_http_client_get_status_code(client));

    esp_http_client_cleanup(client);

    /*-------------------------------------------------------------
     * Parse JSON
     *------------------------------------------------------------*/

    ESP_LOGI(TAG,
             "Server Response:");
    ESP_LOGI(TAG,
             "%s",
             response_buffer);

    cJSON *json =
        cJSON_Parse(response_buffer);

    if (json == NULL)
    {
        ESP_LOGE(TAG,
                 "JSON Parse Failed");

        return ESP_FAIL;
    }

    cJSON *text =
        cJSON_GetObjectItem(
            json,
            "text");

    if ((text == NULL) ||
        (!cJSON_IsString(text)))
    {
        cJSON_Delete(json);

        ESP_LOGE(TAG,
                 "No 'text' field found");

        return ESP_FAIL;
    }

    strncpy(recognized_text,
            text->valuestring,
            max_len - 1);

    recognized_text[max_len - 1] = '\0';

    cJSON_Delete(json);

    ESP_LOGI(TAG,
             "Recognized Text:");
    ESP_LOGI(TAG,
             "%s",
             recognized_text);

    return ESP_OK;
}
