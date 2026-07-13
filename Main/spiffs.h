/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 4
 * SPIFFS Filesystem
 * ============================================================
 */

#ifndef SPIFFS_H
#define SPIFFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------
 * Includes
 *------------------------------------------------------------*/

#include "esp_err.h"

/*-------------------------------------------------------------
 * Public API
 *------------------------------------------------------------*/

/**
 * @brief Initialize SPIFFS
 *
 * Mounts the SPI Flash File System at /spiffs.
 *
 * @return
 *      ESP_OK on success
 *      Error code otherwise
 */
esp_err_t spiffs_init(void);

/**
 * @brief Print SPIFFS usage information
 */
void spiffs_print_info(void);

#ifdef __cplusplus
}
#endif

#endif /* SPIFFS_H */
