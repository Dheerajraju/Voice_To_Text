/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 3
 * Microphone Driver
 * ============================================================
 */

#include "mic.h"

#include <string.h>
#include <math.h>

#include "esp_log.h"
#include "esp_codec_dev.h"
#include "esp_codec_dev_types.h"

#include "bsp/esp-bsp.h"

static const char *TAG = "MIC";

/*-------------------------------------------------------------
 * Private Variables
 *------------------------------------------------------------*/

static esp_codec_dev_handle_t mic_codec = NULL;

static int16_t audio_buffer[AUDIO_BUFFER_SAMPLES];

static uint32_t sample_count = 0;

static float rms_value = 0.0f;

static int16_t peak_value = 0;

static bool voice_detected = false;

/*-------------------------------------------------------------
 * Analyze Audio
 *------------------------------------------------------------*/

static void analyze_audio(void)
{
    int64_t sum = 0;

    peak_value = 0;

    for (uint32_t i = 0; i < sample_count; i++)
    {
        int16_t s = audio_buffer[i];

        if (abs(s) > peak_value)
        {
            peak_value = abs(s);
        }

        sum += ((int32_t)s * (int32_t)s);
    }

    if (sample_count > 0)
    {
        rms_value = sqrt((double)sum / sample_count);
    }
    else
    {
        rms_value = 0;
    }

    voice_detected = (rms_value >= MIC_RMS_THRESHOLD);

    ESP_LOGI(TAG, "========== AUDIO ANALYSIS ==========");
    ESP_LOGI(TAG, "Samples        : %lu", (unsigned long)sample_count);
    ESP_LOGI(TAG, "Peak           : %d", peak_value);
    ESP_LOGI(TAG, "RMS            : %.2f", rms_value);
    ESP_LOGI(TAG, "Voice Detected : %s",
             voice_detected ? "YES" : "NO");
    ESP_LOGI(TAG, "====================================");
}

/*-------------------------------------------------------------
 * Initialize
 *------------------------------------------------------------*/

esp_err_t mic_init(void)
{
    ESP_LOGI(TAG, "Initializing microphone...");

    esp_err_t ret = bsp_audio_init(NULL);

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
        ESP_LOGE(TAG, "esp_codec_dev_open failed");
        return ESP_FAIL;
    }

    esp_codec_dev_set_in_gain(mic_codec, 30.0f);

    ESP_LOGI(TAG, "Microphone initialized successfully");

    return ESP_OK;
}

/*-------------------------------------------------------------
 * Record Audio
 *------------------------------------------------------------*/

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
        uint32_t remain = AUDIO_BUFFER_SAMPLES - sample_count;

        uint32_t block = (remain > 1024) ? 1024 : remain;

        esp_err_t ret =
            esp_codec_dev_read(
                mic_codec,
                &audio_buffer[sample_count],
                block * sizeof(int16_t));

        if (ret != ESP_CODEC_DEV_OK)
        {
            ESP_LOGE(TAG, "Audio read failed");
            return ESP_FAIL;
        }

        sample_count += block;
    }

    ESP_LOGI(TAG,
             "Captured %lu samples",
             (unsigned long)sample_count);

    analyze_audio();

    return ESP_OK;
}

/*-------------------------------------------------------------
 * Stop
 *------------------------------------------------------------*/

esp_err_t mic_stop(void)
{
    if (mic_codec)
    {
        esp_codec_dev_close(mic_codec);
    }

    ESP_LOGI(TAG, "Recording stopped");

    return ESP_OK;
}

/*-------------------------------------------------------------
 * Get Buffer
 *------------------------------------------------------------*/

int16_t *mic_get_buffer(void)
{
    return audio_buffer;
}

/*-------------------------------------------------------------
 * Get Sample Count
 *------------------------------------------------------------*/

uint32_t mic_get_sample_count(void)
{
    return sample_count;
}

/*-------------------------------------------------------------
 * Get RMS
 *------------------------------------------------------------*/

float mic_get_rms(void)
{
    return rms_value;
}

/*-------------------------------------------------------------
 * Get Peak
 *------------------------------------------------------------*/

int16_t mic_get_peak(void)
{
    return peak_value;
}

/*-------------------------------------------------------------
 * Voice Detection
 *------------------------------------------------------------*/

bool mic_voice_detected(void)
{
    return voice_detected;
}
