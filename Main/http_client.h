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
#include <stdbool.h>
#include <stddef.h>
#include "esp_err.h"

/*-------------------------------------------------------------
 * Configuration
 *------------------------------------------------------------*/
/* Replace with your computer's local Wi-Fi IP address and server port */
#define TRANSCRIBE_URL              "http://10.157.42.233:8000/transcribe"

/* Buffer size for receiving JSON response from server */
#define HTTP_RESPONSE_BUFFER_SIZE   8192

/*-------------------------------------------------------------
 * Public API
 *------------------------------------------------------------*/

/**
 * @brief Upload /spiffs/record.wav to local Whisper server via HTTP POST.
 *
 * Streams the audio file using multipart/form-data and parses the
 * JSON response {"text": "..."}.
 *
 * @param[out] recognized_text Buffer to store the transcribed text string.
 * @param[in]  max_len         Maximum capacity of recognized_text buffer.
 *
 * @return ESP_OK on success, ESP_FAIL or appropriate error code otherwise.
 */
esp_err_t http_upload_wav(char *recognized_text, size_t max_len);

/**
 * @brief Check if the speech recognition server is reachable.
 *
 * @return true if server responds to a GET request, false otherwise.
 */
bool http_server_available(void);

#ifdef __cplusplus
}
#endif

#endif /* HTTP_CLIENT_H */
