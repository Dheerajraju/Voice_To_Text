/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 6.1
 * Microphone Driver Header
 * ============================================================
 */

#ifndef MIC_H
#define MIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "esp_err.h"

/*----------------------------------------------------------
 * Audio Configuration
 *---------------------------------------------------------*/

#define SAMPLE_RATE             16000
#define RECORD_TIME_SECONDS     5

#define AUDIO_BUFFER_SAMPLES \
    (SAMPLE_RATE * RECORD_TIME_SECONDS)

/*----------------------------------------------------------
 * Audio Statistics
 *---------------------------------------------------------*/

typedef struct
{
    uint32_t sample_count;

    int16_t peak;

    float rms;

    uint8_t voice_detected;

} mic_stats_t;

/*----------------------------------------------------------
 * Driver Functions
 *---------------------------------------------------------*/

esp_err_t mic_init(void);

esp_err_t mic_start(void);

esp_err_t mic_stop(void);

/*----------------------------------------------------------
 * Audio Buffer
 *---------------------------------------------------------*/

int16_t *mic_get_buffer(void);

uint32_t mic_get_sample_count(void);

/*----------------------------------------------------------
 * Audio Analysis
 *---------------------------------------------------------*/

esp_err_t mic_analyze(mic_stats_t *stats);

#ifdef __cplusplus
}
#endif

#endif
