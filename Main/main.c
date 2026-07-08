#include "bsp/esp32_p4_wifi6_touch_lcd_xc.h"

#include "lvgl.h"

#include "esp_log.h"

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

    lv_display_t *display = bsp_display_start_with_config(&cfg);

    if(display == NULL)
    {
        ESP_LOGE(TAG, "Display init failed");

        while(1);
    }

    bsp_display_backlight_on();

    bsp_display_lock(portMAX_DELAY);

    lv_obj_t *scr = lv_screen_active();

    lv_obj_set_style_bg_color(
        scr,
        lv_color_white(),
        0);

    lv_obj_t *label = lv_label_create(scr);

    lv_label_set_text(
        label,
        "Hello VTT_V2");

    lv_obj_set_style_text_font(
        label,
        &lv_font_montserrat_30,
        0);

    lv_obj_set_style_text_color(
        label,
        lv_color_black(),
        0);

    lv_obj_center(label);

    bsp_display_unlock();

    ESP_LOGI(TAG, "Display OK");

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
