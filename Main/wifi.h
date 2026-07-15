/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * WiFi Manager Header
 * ============================================================
 */

#ifndef WIFI_H
#define WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Includes
 *------------------------------------------------------------*/

#include "esp_err.h"

/*-------------------------------------------------------------
 * WiFi Configuration
 *------------------------------------------------------------*/

#define WIFI_SSID       "Dheeraj_VTT_V8"
#define WIFI_PASSWORD   "1234567890"

/*
 * Whisper Server IP
 *
 * Replace this if your PC IP changes.
 */
#define SERVER_IP       "10.157.42.233"
#define SERVER_PORT     8000

/* Complete transcription URL */
#define TRANSCRIBE_URL  "http://10.157.42.233:8000/transcribe"

/*-------------------------------------------------------------
 * Public API
 *------------------------------------------------------------*/

/**
 * @brief Connect ESP32 to WiFi
 *
 * @return ESP_OK on success
 */
esp_err_t wifi_init_sta(void);

#ifdef __cplusplus
}
#endif

#endif
