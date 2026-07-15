/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Speech Recognition Module
 * ============================================================
 */

#include "speech.h"
#include "http_client.h"

#include <string.h>

#include "esp_log.h"

static const char *TAG = "SPEECH";

/*-------------------------------------------------------------
 * Initialize Speech Module
 *------------------------------------------------------------*/

esp_err_t speech_init(void)
{
    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Initializing Speech Module");
    ESP_LOGI(TAG, "====================================");

    return ESP_OK;
}

/*-------------------------------------------------------------
 * Upload WAV to Whisper Server
 *------------------------------------------------------------*/

esp_err_t speech_recognize(char *text, size_t max_len)
{
    if (text == NULL || max_len == 0)
    {
        ESP_LOGE(TAG, "Invalid arguments");
        return ESP_ERR_INVALID_ARG;
    }

    memset(text, 0, max_len);

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Uploading WAV to Whisper Server...");
    ESP_LOGI(TAG, "====================================");

    esp_err_t err = http_upload_wav(text, max_len);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Speech recognition failed (%s)", esp_err_to_name(err));

        strncpy(text, "Recognition Failed", max_len - 1);
        text[max_len - 1] = '\0';

        return err;
    }

    if (strlen(text) == 0)
    {
        strncpy(text, "No speech recognized", max_len - 1);
        text[max_len - 1] = '\0';
    }

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Speech Recognition Complete");
    ESP_LOGI(TAG, "Recognized Text:");
    ESP_LOGI(TAG, "%s", text);
    ESP_LOGI(TAG, "====================================");

    return ESP_OK;
}
