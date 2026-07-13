/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Stage 4.2
 * Main Application
 * ============================================================
 */

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_err.h"

#include "lvgl.h"

#include "bsp/esp-bsp.h"

#include "ui.h"
#include "mic.h"
#include "spiffs.h"

static const char *TAG = "VTT";

void app_main(void)
{
    esp_err_t ret;

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "ESP32-P4 AI Voice Assistant");
    ESP_LOGI(TAG, "VTT_V8");
    ESP_LOGI(TAG, "Stage 4.2");
    ESP_LOGI(TAG, "====================================");

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
    bsp_display_brightness_set(100);

    ESP_LOGI(TAG, "LCD Initialized");

    /*---------------------------------------------------------
     * SPIFFS
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing SPIFFS...");

    ret = spiffs_init();

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "SPIFFS initialization failed");
        return;
    }

    ESP_LOGI(TAG, "SPIFFS Initialized");

    /*---------------------------------------------------------
     * Microphone
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Initializing Microphone...");

    ret = mic_init();

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Microphone initialization failed");
        return;
    }

    ESP_LOGI(TAG, "Microphone initialized successfully");

    /*---------------------------------------------------------
     * UI
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Creating Home Screen");

    bsp_display_lock(portMAX_DELAY);

    ui_init();

    lv_obj_invalidate(lv_screen_active());
    lv_refr_now(display);

    bsp_display_unlock();

    ESP_LOGI(TAG, "UI Initialized");

    /*---------------------------------------------------------
     * Main Loop
     *---------------------------------------------------------*/

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
