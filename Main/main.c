/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Stage 3
 * Main Application
 * ============================================================
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "bsp/esp-bsp.h"

#include "ui.h"
#include "mic.h"

static const char *TAG = "VTT";

void app_main(void)
{
    ESP_LOGI(TAG, "=================================");
    ESP_LOGI(TAG, "ESP32-P4 AI Voice Assistant");
    ESP_LOGI(TAG, "VTT_V8 Stage 3");
    ESP_LOGI(TAG, "=================================");

    /*----------------------------------------------------------
     * Initialize LCD
     *---------------------------------------------------------*/

    bsp_display_cfg_t cfg = {
        .lv_adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG(),

        .rotation = ESP_LV_ADAPTER_ROTATE_0,

        .tear_avoid_mode =
            ESP_LV_ADAPTER_TEAR_AVOID_MODE_TRIPLE_PARTIAL,

        .touch_flags = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        },
    };

    lv_display_t *display = bsp_display_start_with_config(&cfg);

    if (display == NULL)
    {
        ESP_LOGE(TAG, "Display initialization failed");

        while (1)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    bsp_display_backlight_on();

    ESP_LOGI(TAG, "LCD Initialized");

    /*----------------------------------------------------------
     * Initialize Microphone
     *---------------------------------------------------------*/

    if (mic_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Microphone initialization failed");
    }
    else
    {
        ESP_LOGI(TAG, "Microphone initialized successfully");
    }

    /*----------------------------------------------------------
     * Create UI
     *---------------------------------------------------------*/

    bsp_display_lock(portMAX_DELAY);

    ui_init();

    bsp_display_unlock();

    ESP_LOGI(TAG, "UI Initialized");

    /*----------------------------------------------------------
     * Main Loop
     *---------------------------------------------------------*/

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
