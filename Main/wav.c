/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 4
 * WAV File Generator
 * ============================================================
 */

#include "wav.h"

#include <stdio.h>
#include <string.h>

#include "esp_log.h"

static const char *TAG = "WAV";

/*-------------------------------------------------------------
 * Create WAV Header
 *------------------------------------------------------------*/

void wav_create_header(wav_header_t *header,
                       uint32_t sample_count)
{
    uint32_t data_size = sample_count * sizeof(int16_t);

    memcpy(header->riff, "RIFF", 4);
    memcpy(header->wave, "WAVE", 4);
    memcpy(header->fmt,  "fmt ", 4);
    memcpy(header->data, "data", 4);

    header->chunk_size = 36 + data_size;

    header->subchunk1_size = 16;

    header->audio_format = 1;             // PCM

    header->num_channels = WAV_CHANNELS;

    header->sample_rate = WAV_SAMPLE_RATE;

    header->bits_per_sample = WAV_BITS_PER_SAMPLE;

    header->byte_rate =
        WAV_SAMPLE_RATE *
        WAV_CHANNELS *
        (WAV_BITS_PER_SAMPLE / 8);

    header->block_align =
        WAV_CHANNELS *
        (WAV_BITS_PER_SAMPLE / 8);

    header->data_size = data_size;
}

/*-------------------------------------------------------------
 * Save WAV File
 *------------------------------------------------------------*/

esp_err_t wav_save_file(const char *filename,
                        const int16_t *pcm_data,
                        uint32_t sample_count)
{
    if (filename == NULL)
    {
        ESP_LOGE(TAG, "Filename is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    if (pcm_data == NULL)
    {
        ESP_LOGE(TAG, "PCM buffer is NULL");
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Creating WAV file...");

    FILE *fp = fopen(filename, "wb");

    if (fp == NULL)
    {
        ESP_LOGE(TAG, "Cannot create %s", filename);
        return ESP_FAIL;
    }

    wav_header_t header;

    wav_create_header(&header, sample_count);

    ESP_LOGI(TAG, "Writing WAV header...");

    fwrite(&header,
           sizeof(wav_header_t),
           1,
           fp);

    ESP_LOGI(TAG,
             "Writing %lu samples...",
             (unsigned long)sample_count);

    fwrite(pcm_data,
           sizeof(int16_t),
           sample_count,
           fp);

    fclose(fp);

    ESP_LOGI(TAG, "================================");
    ESP_LOGI(TAG, "WAV saved successfully");
    ESP_LOGI(TAG, "File : %s", filename);
    ESP_LOGI(TAG, "Samples : %lu",
             (unsigned long)sample_count);
    ESP_LOGI(TAG, "Rate : %d Hz", WAV_SAMPLE_RATE);
    ESP_LOGI(TAG, "Bits : %d", WAV_BITS_PER_SAMPLE);
    ESP_LOGI(TAG, "Channels : %d", WAV_CHANNELS);
    ESP_LOGI(TAG, "================================");

    return ESP_OK;
}
