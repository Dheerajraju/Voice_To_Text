/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * HTTP Upload Client (Streaming Chunked Version)
 * ============================================================
 */

#include "http_client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "HTTP";

/*-------------------------------------------------------------
 * Private Response Buffer
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
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;

        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "Connected to Whisper server");
            break;

        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP header sent");
            break;

        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "Header: %s = %s", evt->header_key, evt->header_value);
            break;

        case HTTP_EVENT_ON_DATA:
            /* Accumulate response payload into our buffer */
            if (!esp_http_client_is_chunked_response(evt->client))
            {
                if ((response_length + evt->data_len) < HTTP_RESPONSE_BUFFER_SIZE)
                {
                    memcpy(response_buffer + response_length, evt->data, evt->data_len);
                    response_length += evt->data_len;
                    response_buffer[response_length] = '\0';
                }
                else
                {
                    ESP_LOGW(TAG, "Response buffer overflow!");
                }
            }
            break;

        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP session finished");
            break;

        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected from server");
            break;

        default:
            break;
    }
    return ESP_OK;
}

/*-------------------------------------------------------------
 * Check Server Availability
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

    /* Reset output string */
    recognized_text[0] = '\0';

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Starting Audio Upload to Server...");
    ESP_LOGI(TAG, "Target URL : %s", TRANSCRIBE_URL);
    ESP_LOGI(TAG, "====================================");

    FILE *fp = fopen("/spiffs/record.wav", "rb");
    if (fp == NULL)
    {
        ESP_LOGE(TAG, "Failed to open /spiffs/record.wav for reading");
        return ESP_FAIL;
    }

    /*-------------------------------------------------------------
     * 1. Validate File Size
     *------------------------------------------------------------*/
    fseek(fp, 0, SEEK_END);
    long wav_size = ftell(fp);
    rewind(fp);

    ESP_LOGI(TAG, "WAV File Size : %ld bytes", wav_size);

    /* Guard against uploading empty or truncated SPIFFS files */
    if (wav_size < 1000)
    {
        ESP_LOGE(TAG, "WAV file is too small (%ld bytes). Audio recording or SPIFFS save failed!", wav_size);
        fclose(fp);
        return ESP_FAIL;
    }

    response_length = 0;
    memset(response_buffer, 0, sizeof(response_buffer));

    /*-------------------------------------------------------------
     * 2. Build Multipart Form-Data Headers
     *------------------------------------------------------------*/
    const char *boundary = "------------------------ESP32P4AudioBoundary";
    char header[256];
    char footer[128];

    int header_len = snprintf(header, sizeof(header),
                              "--%s\r\n"
                              "Content-Disposition: form-data; name=\"file\"; filename=\"record.wav\"\r\n"
                              "Content-Type: audio/wav\r\n\r\n",
                              boundary);

    int footer_len = snprintf(footer, sizeof(footer),
                              "\r\n--%s--\r\n",
                              boundary);

    int total_post_size = header_len + (int)wav_size + footer_len;
    ESP_LOGI(TAG, "Total POST Content-Length : %d bytes", total_post_size);

    /*-------------------------------------------------------------
     * 3. Initialize HTTP Client
     *------------------------------------------------------------*/
    esp_http_client_config_t config = {
        .url = TRANSCRIBE_URL,
        .method = HTTP_METHOD_POST,
        .timeout_ms = 60000, /* 60 seconds timeout for AI transcription */
        .event_handler = http_event_handler,
        .buffer_size = HTTP_RESPONSE_BUFFER_SIZE, /* Fixed: changed from buffer_size_rx to buffer_size */
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialize HTTP client");
        fclose(fp);
        return ESP_FAIL;
    }

    char content_type[128];
    snprintf(content_type, sizeof(content_type), "multipart/form-data; boundary=%s", boundary);
    esp_http_client_set_header(client, "Content-Type", content_type);

    /* Open streaming connection */
    esp_err_t err = esp_http_client_open(client, total_post_size);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        fclose(fp);
        return err;
    }

    /*-------------------------------------------------------------
     * 4. Stream Payload: Header -> Audio Chunks -> Footer
     *------------------------------------------------------------*/
    /* Write multipart header */
    if (esp_http_client_write(client, header, header_len) < 0)
    {
        ESP_LOGE(TAG, "Failed writing multipart header");
        esp_http_client_cleanup(client);
        fclose(fp);
        return ESP_FAIL;
    }

    /* Write WAV file in 4 KB chunks */
    const size_t chunk_size = 4096;
    char *chunk_buffer = (char *)malloc(chunk_size);
    if (chunk_buffer == NULL)
    {
        ESP_LOGE(TAG, "Memory allocation failed for audio stream buffer");
        esp_http_client_cleanup(client);
        fclose(fp);
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(TAG, "Streaming audio samples to server...");
    size_t read_bytes = 0;
    size_t total_sent = 0;

    while ((read_bytes = fread(chunk_buffer, 1, chunk_size, fp)) > 0)
    {
        int written = esp_http_client_write(client, chunk_buffer, read_bytes);
        if (written < 0)
        {
            ESP_LOGE(TAG, "Audio upload stream interrupted at %zu bytes", total_sent);
            free(chunk_buffer);
            fclose(fp);
            esp_http_client_cleanup(client);
            return ESP_FAIL;
        }
        total_sent += written;
    }

    free(chunk_buffer);
    fclose(fp);
    ESP_LOGI(TAG, "Audio streaming complete (%zu bytes sent)", total_sent);

    /* Write multipart footer */
    if (esp_http_client_write(client, footer, footer_len) < 0)
    {
        ESP_LOGE(TAG, "Failed writing multipart footer");
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    /*-------------------------------------------------------------
     * 5. Fetch Server Response
     *------------------------------------------------------------*/
    int content_length = esp_http_client_fetch_headers(client);
    int status_code = esp_http_client_get_status_code(client);
    ESP_LOGI(TAG, "HTTP Status Code : %d, Response Length : %d", status_code, content_length);

    if (status_code != 200)
    {
        ESP_LOGE(TAG, "Server returned HTTP error code: %d", status_code);
        esp_http_client_cleanup(client);
        return ESP_FAIL;
    }

    /* If event handler didn't capture data, read manually from stream */
    if (response_length == 0)
    {
        int len = esp_http_client_read_response(client, response_buffer, sizeof(response_buffer) - 1);
        if (len > 0)
        {
            response_length = len;
            response_buffer[response_length] = '\0';
        }
    }

    esp_http_client_cleanup(client);

    ESP_LOGI(TAG, "Raw Server Response:\n%s", response_buffer);

    /*-------------------------------------------------------------
     * 6. Parse JSON {"text": "..."}
     *------------------------------------------------------------*/
    cJSON *json = cJSON_Parse(response_buffer);
    if (json == NULL)
    {
        ESP_LOGE(TAG, "Failed to parse JSON response from server");
        return ESP_FAIL;
    }

    cJSON *text_item = cJSON_GetObjectItemCaseSensitive(json, "text");
    if (text_item == NULL || !cJSON_IsString(text_item) || (text_item->valuestring == NULL))
    {
        ESP_LOGE(TAG, "JSON response missing valid 'text' field");
        cJSON_Delete(json);
        return ESP_FAIL;
    }

    /* Copy transcribed string safely to caller's buffer */
    strncpy(recognized_text, text_item->valuestring, max_len - 1);
    recognized_text[max_len - 1] = '\0';

    cJSON_Delete(json);

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Transcription Successful!");
    ESP_LOGI(TAG, "Recognized Text: \"%s\"", recognized_text);
    ESP_LOGI(TAG, "====================================");

    return ESP_OK;
}
