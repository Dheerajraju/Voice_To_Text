/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Speech Recognition Module
 * ============================================================
 */

#ifndef SPEECH_H
#define SPEECH_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Includes
 *------------------------------------------------------------*/

#include "esp_err.h"
#include <stddef.h>

/*-------------------------------------------------------------
 * Public API
 *------------------------------------------------------------*/

/**
 * @brief Initialize Speech Recognition Module
 *
 * Currently performs any future initialization.
 *
 * @return ESP_OK on success
 */
esp_err_t speech_init(void);

/**
 * @brief Upload recorded WAV file to Whisper server
 *        and return recognized text.
 *
 * @param text
 * Output buffer.
 *
 * @param max_len
 * Size of output buffer.
 *
 * @return
 * ESP_OK on success
 */
esp_err_t speech_recognize(char *text,
                           size_t max_len);

#ifdef __cplusplus
}
#endif

#endif
