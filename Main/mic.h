/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 3
 * Microphone Driver Header
 * ============================================================
 */

#ifndef MIC_H
#define MIC_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Includes
 *------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>

#include "esp_err.h"

/*-------------------------------------------------------------
 * Audio Configuration
 *------------------------------------------------------------*/

#define SAMPLE_RATE             16000
#define BITS_PER_SAMPLE         16
#define CHANNELS                1

/* 5 seconds of audio */
#define RECORD_TIME_SECONDS     5

/* Total samples */
#define AUDIO_BUFFER_SAMPLES    (16000 * 5)

/* Total PCM buffer size */
#define AUDIO_BUFFER_SIZE        (AUDIO_BUFFER_SAMPLES * sizeof(int16_t))

/* Voice detection threshold */
#define MIC_RMS_THRESHOLD        500

/*-------------------------------------------------------------
 * Public API
 *------------------------------------------------------------*/

/**
 * @brief Initialize microphone
 *
 * @return ESP_OK on success
 */
esp_err_t mic_init(void);

/**
 * @brief Record audio
 *
 * Blocking call.
 *
 * @return ESP_OK on success
 */
esp_err_t mic_start(void);

/**
 * @brief Stop microphone
 *
 * @return ESP_OK on success
 */
esp_err_t mic_stop(void);

/**
 * @brief Get PCM audio buffer
 *
 * @return Pointer to recorded samples
 */
int16_t *mic_get_buffer(void);

/**
 * @brief Get number of captured samples
 *
 * @return Sample count
 */
uint32_t mic_get_sample_count(void);

/**
 * @brief Get RMS value
 *
 * @return RMS amplitude
 */
float mic_get_rms(void);

/**
 * @brief Get Peak amplitude
 *
 * @return Peak amplitude
 */
int16_t mic_get_peak(void);

/**
 * @brief Voice detection result
 *
 * @return true if voice detected
 */
bool mic_voice_detected(void);

#ifdef __cplusplus
}
#endif

#endif /* MIC_H */
