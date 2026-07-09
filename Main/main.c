#include "bsp/esp32_p4_wifi6_touch_lcd_xc.h"
#include "ui.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    bsp_display_cfg_t cfg = {
        .lv_adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG(),
        .rotation = ESP_LV_ADAPTER_ROTATE_0,
        .tear_avoid_mode = ESP_LV_ADAPTER_TEAR_AVOID_MODE_NONE,
        .touch_flags = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        },
    };

    if (bsp_display_start_with_config(&cfg) == NULL)
    {
        ESP_LOGE(TAG, "Display init failed");

        while (1)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    bsp_display_backlight_on();

    bsp_display_lock(portMAX_DELAY);

    ui_create();

    bsp_display_unlock();

    ESP_LOGI(TAG, "Application Started");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
