/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 4
 * WAV File Generator
 * ============================================================
 */

#ifndef WAV_H
#define WAV_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Includes
 *------------------------------------------------------------*/

#include <stdint.h>
#include "esp_err.h"

/*-------------------------------------------------------------
 * Audio Format
 *------------------------------------------------------------*/

#define WAV_SAMPLE_RATE        16000
#define WAV_BITS_PER_SAMPLE    16
#define WAV_CHANNELS           1

/*-------------------------------------------------------------
 * Default Output File
 *------------------------------------------------------------*/

#define WAV_FILE_PATH "/spiffs/record.wav"

/*-------------------------------------------------------------
 * WAV Header
 *------------------------------------------------------------*/

typedef struct
{
    char     riff[4];              // "RIFF"
    uint32_t chunk_size;
    char     wave[4];              // "WAVE"

    char     fmt[4];               // "fmt "
    uint32_t subchunk1_size;
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;

    char     data[4];              // "data"
    uint32_t data_size;

} wav_header_t;

/*-------------------------------------------------------------
 * Public API
 *------------------------------------------------------------*/

/**
 * @brief Save PCM buffer as a WAV file.
 *
 * @param filename Output filename.
 * @param pcm_data Pointer to 16-bit PCM samples.
 * @param sample_count Number of samples.
 *
 * @return ESP_OK on success.
 */
esp_err_t wav_save_file(const char *filename,
                        const int16_t *pcm_data,
                        uint32_t sample_count);

/**
 * @brief Create a WAV header.
 *
 * @param header Pointer to header structure.
 * @param sample_count Number of PCM samples.
 */
void wav_create_header(wav_header_t *header,
                       uint32_t sample_count);

#ifdef __cplusplus
}
#endif

#endif /* WAV_H */
