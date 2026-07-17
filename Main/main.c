/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Main Application
 *
 * Compatible with:
 *  ESP-IDF v5.5.4
 *  Waveshare BSP v2.0.0
 *  LVGL v9.5
 * ============================================================
 */

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "nvs_flash.h"

#include "bsp/esp-bsp.h"

#include "lvgl.h"

#include "ui.h"
#include "ui_events.h"

#include "wifi.h"
#include "mic.h"
#include "speech.h"
#include "spiffs.h"

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
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ESP_LOGI(TAG, "NVS Ready");

    /*---------------------------------------------------------
     * LCD
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing Display...");

    lv_display_t *display = bsp_display_start();

    if (display == NULL)
    {
        ESP_LOGE(TAG, "Display initialization failed");
        return;
    }

    bsp_display_brightness_init();
    bsp_display_backlight_on();
    bsp_display_brightness_set(100);

    ESP_LOGI(TAG, "Display Ready");

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

    ESP_LOGI(TAG, "SPIFFS Ready");

    /*---------------------------------------------------------
     * WiFi
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing WiFi...");

    err = wifi_init_sta();

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "WiFi Connected");
    }
    else
    {
        ESP_LOGW(TAG, "WiFi unavailable");
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

    ESP_LOGI(TAG, "Microphone Ready");

    /*---------------------------------------------------------
     * Speech Module
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing Speech Module...");

    err = speech_init();

    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "Speech module initialization failed");
    }

    ESP_LOGI(TAG, "Speech Module Ready");

    /*---------------------------------------------------------
     * UI
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Creating User Interface...");

    /*
     * IMPORTANT
     *
     * All LVGL object creation must happen while
     * holding the BSP display lock.
     */

    bsp_display_lock(portMAX_DELAY);

    ui_init();
    ui_events_init();

    bsp_display_unlock();

    ESP_LOGI(TAG, "UI Ready");

    /*---------------------------------------------------------
     * System Ready
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "System Ready");
    ESP_LOGI(TAG, "Touch the blue button to start.");
    ESP_LOGI(TAG, "====================================");

    /*
     * The Waveshare BSP already owns the LVGL task.
     * Nothing else is required here.
     */

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
