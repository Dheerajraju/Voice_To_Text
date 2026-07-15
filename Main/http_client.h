/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * HTTP Client Header
 * ============================================================
 */

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Includes
 *------------------------------------------------------------*/

#include "esp_err.h"
#include <stdbool.h>

/*-------------------------------------------------------------
 * Configuration
 *------------------------------------------------------------*/

#define HTTP_RESPONSE_BUFFER_SIZE    8192

/*-------------------------------------------------------------
 * Public API
 *------------------------------------------------------------*/

/**
 * @brief Upload record.wav to Whisper server
 *
 * Opens:
 *      /spiffs/record.wav
 *
 * Sends:
 *      multipart/form-data POST
 *
 * URL:
 *      http://SERVER_IP:8000/transcribe
 *
 * @param recognized_text
 * Buffer where the recognized text will be copied.
 *
 * @param max_len
 * Size of recognized_text buffer.
 *
 * @return
 * ESP_OK on success.
 */
esp_err_t http_upload_wav(char *recognized_text,
                          size_t max_len);

/**
 * @brief Check if server is reachable
 *
 * Optional helper function.
 */
bool http_server_available(void);

#ifdef __cplusplus
}
#endif

#endif
