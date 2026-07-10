/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 6.1
 * Microphone Driver
 * ES7210 (Waveshare BSP)
 * ============================================================
 */

#include "mic.h"

#include <string.h>
#include <math.h>

#include "esp_log.h"
#include "esp_err.h"

#include "esp_codec_dev.h"
#include "esp_codec_dev_types.h"

#include "bsp/esp-bsp.h"

static const char *TAG = "MIC";

/*----------------------------------------------------------
 * Private Variables
 *---------------------------------------------------------*/

static esp_codec_dev_handle_t mic_codec = NULL;

static int16_t audio_buffer[AUDIO_BUFFER_SAMPLES];

static uint32_t sample_count = 0;

/*----------------------------------------------------------
 * Initialize
 *---------------------------------------------------------*/

esp_err_t mic_init(void)
{
    ESP_LOGI(TAG, "Initializing microphone...");

    esp_err_t ret;

    ret = bsp_audio_init(NULL);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "bsp_audio_init failed");
        return ret;
    }

    mic_codec = bsp_audio_codec_microphone_init();

    if (mic_codec == NULL)
    {
        ESP_LOGE(TAG, "Failed to create microphone codec");
        return ESP_FAIL;
    }

    esp_codec_dev_sample_info_t fs = {
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = 16,
        .channel = 1,
        .channel_mask = 1,
        .mclk_multiple = 256
    };

    ret = esp_codec_dev_open(mic_codec, &fs);

    if (ret != ESP_CODEC_DEV_OK)
    {
        ESP_LOGE(TAG,
                 "esp_codec_dev_open failed (%s)",
                 esp_err_to_name(ret));

        return ret;
    }

    ret = esp_codec_dev_set_in_gain(mic_codec, 30.0f);

    if (ret != ESP_CODEC_DEV_OK)
    {
        ESP_LOGW(TAG, "Unable to set microphone gain");
    }

    ESP_LOGI(TAG, "Microphone initialized successfully");

    return ESP_OK;
}

/*----------------------------------------------------------
 * Record Audio
 *---------------------------------------------------------*/

esp_err_t mic_start(void)
{
    if (mic_codec == NULL)
    {
        ESP_LOGE(TAG, "Microphone not initialized");
        return ESP_FAIL;
    }

    memset(audio_buffer, 0, sizeof(audio_buffer));

    sample_count = 0;

    ESP_LOGI(TAG, "Recording...");

    while (sample_count < AUDIO_BUFFER_SAMPLES)
    {
        uint32_t remaining = AUDIO_BUFFER_SAMPLES - sample_count;

        uint32_t samples = (remaining > 1024) ? 1024 : remaining;

        esp_err_t ret = esp_codec_dev_read(
            mic_codec,
            &audio_buffer[sample_count],
            samples * sizeof(int16_t));

        if (ret != ESP_CODEC_DEV_OK)
        {
            ESP_LOGE(TAG,
                     "Read failed (%s)",
                     esp_err_to_name(ret));

            return ret;
        }

        sample_count += samples;
    }

    ESP_LOGI(TAG,
             "Captured %lu samples",
             (unsigned long)sample_count);

    mic_stats_t stats;

    mic_analyze(&stats);

    return ESP_OK;
}

/*----------------------------------------------------------
 * Analyze Audio
 *---------------------------------------------------------*/

esp_err_t mic_analyze(mic_stats_t *stats)
{
    if (stats == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }

    int32_t peak = 0;

    double sum = 0.0;

    for (uint32_t i = 0; i < sample_count; i++)
    {
        int32_t sample = audio_buffer[i];

        if (sample < 0)
            sample = -sample;

        if (sample > peak)
            peak = sample;

        sum += ((double)sample * (double)sample);
    }

    float rms = sqrt(sum / sample_count);

    stats->sample_count = sample_count;
    stats->peak = peak;
    stats->rms = rms;

    if (rms > 500.0f)
        stats->voice_detected = 1;
    else
        stats->voice_detected = 0;

    ESP_LOGI(TAG, "========== AUDIO ANALYSIS ==========");
    ESP_LOGI(TAG, "Samples        : %lu",
             (unsigned long)stats->sample_count);
    ESP_LOGI(TAG, "Peak           : %d",
             stats->peak);
    ESP_LOGI(TAG, "RMS            : %.2f",
             stats->rms);
    ESP_LOGI(TAG, "Voice Detected : %s",
             stats->voice_detected ? "YES" : "NO");
    ESP_LOGI(TAG, "====================================");

    return ESP_OK;
}

/*----------------------------------------------------------
 * Stop
 *---------------------------------------------------------*/

esp_err_t mic_stop(void)
{
    if (mic_codec)
    {
        esp_codec_dev_close(mic_codec);
    }

    ESP_LOGI(TAG, "Recording stopped");

    return ESP_OK;
}

/*----------------------------------------------------------
 * Buffer
 *---------------------------------------------------------*/

int16_t *mic_get_buffer(void)
{
    return audio_buffer;
}

/*----------------------------------------------------------
 * Sample Count
 *---------------------------------------------------------*/

uint32_t mic_get_sample_count(void)
{
    return sample_count;
}
