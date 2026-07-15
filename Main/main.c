/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Stage 5
 * Main Application  DESKTOP-G3VO5OF 9249
 * ============================================================
 */

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_err.h"

#include "nvs_flash.h"

#include "bsp/esp-bsp.h"
#include "lvgl.h"

#include "ui.h"
#include "ui_events.h"

#include "mic.h"
#include "wifi.h"
#include "spiffs.h"
#include "speech.h"

static const char *TAG = "VTT";

/*-------------------------------------------------------------
 * Application Entry
 *------------------------------------------------------------*/

void app_main(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "ESP32-P4 AI Voice Assistant");
    ESP_LOGI(TAG, "VTT_V8");
    ESP_LOGI(TAG, "Stage 5");
    ESP_LOGI(TAG, "====================================");

    /*---------------------------------------------------------
     * NVS
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing NVS...");

    err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
        err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    ESP_ERROR_CHECK(err);

    ESP_LOGI(TAG, "NVS Initialized");

    /*---------------------------------------------------------
     * LCD
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing LCD...");

    lv_display_t *display = bsp_display_start();

    if (display == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialize LCD");
        return;
    }

    ESP_LOGI(TAG, "LCD Initialized");

    /*---------------------------------------------------------
     * SPIFFS
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing SPIFFS...");

    err = spiffs_init();

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "SPIFFS initialization failed");
        return;
    }

    ESP_LOGI(TAG, "SPIFFS Initialized");

    /*---------------------------------------------------------
     * WiFi
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing WiFi...");

    err = wifi_init_sta();

    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "WiFi unavailable.");
        ESP_LOGW(TAG, "Speech recognition disabled.");
    }
    else
    {
        ESP_LOGI(TAG, "WiFi Connected");
    }

    /*---------------------------------------------------------
     * Microphone
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing Microphone...");

    err = mic_init();

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Microphone initialization failed");
        return;
    }

    ESP_LOGI(TAG, "Microphone Initialized");

    /*---------------------------------------------------------
     * Speech Module
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing Speech Module...");

    err = speech_init();

    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "Speech module unavailable.");
    }
    else
    {
        ESP_LOGI(TAG, "Speech Module Initialized");
    }

    /*---------------------------------------------------------
     * UI
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Creating Home Screen...");

    ui_init();
    ui_events_init();

    ESP_LOGI(TAG, "UI Initialized");

    /*---------------------------------------------------------
     * Ready
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "System Ready");
    ESP_LOGI(TAG, "Touch the button to speak.");
    ESP_LOGI(TAG, "====================================");

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
