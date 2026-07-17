/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * UI Manager
 *
 * Compatible with:
 * ESP-IDF v5.5.4
 * LVGL v9.5
 * Waveshare BSP v2.0.0
 * ============================================================
 */

#include "ui.h"
#include "ui_home.h"

#include "bsp/esp-bsp.h"

#include "esp_log.h"

static const char *TAG = "UI";

/*-------------------------------------------------------------
 * Global Objects
 *------------------------------------------------------------*/

lv_obj_t *ui_screen = NULL;

lv_obj_t *ui_title = NULL;

lv_obj_t *ui_mic_btn = NULL;
lv_obj_t *ui_mic_label = NULL;

lv_obj_t *ui_status = NULL;

lv_obj_t *ui_speech_title = NULL;
lv_obj_t *ui_speech_text = NULL;

/*-------------------------------------------------------------
 * Initialize UI
 *------------------------------------------------------------*/

void ui_init(void)
{
    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Creating Home Screen");
    ESP_LOGI(TAG, "====================================");

    ui_home_create();

    if (ui_screen != NULL)
    {
        lv_screen_load(ui_screen);
    }

    ESP_LOGI(TAG, "UI Initialized");
}

/*-------------------------------------------------------------
 * Update Status
 *------------------------------------------------------------*/

void ui_set_status(const char *text)
{
    if (ui_status == NULL || text == NULL)
    {
        return;
    }

    bsp_display_lock(portMAX_DELAY);

    lv_label_set_text(
        ui_status,
        text);

    bsp_display_unlock();
}

/*-------------------------------------------------------------
 * Update Speech Text
 *------------------------------------------------------------*/

void ui_set_speech(const char *text)
{
    if (ui_speech_text == NULL || text == NULL)
    {
        return;
    }

    bsp_display_lock(portMAX_DELAY);

    lv_label_set_text(
        ui_speech_text,
        text);

    bsp_display_unlock();
}

/*-------------------------------------------------------------
 * Enable Button
 *------------------------------------------------------------*/

void ui_enable_button(void)
{
    if (ui_mic_btn == NULL)
    {
        return;
    }

    bsp_display_lock(portMAX_DELAY);

    lv_obj_clear_state(
        ui_mic_btn,
        LV_STATE_DISABLED);

    bsp_display_unlock();
}

/*-------------------------------------------------------------
 * Disable Button
 *------------------------------------------------------------*/

void ui_disable_button(void)
{
    if (ui_mic_btn == NULL)
    {
        return;
    }

    bsp_display_lock(portMAX_DELAY);

    lv_obj_add_state(
        ui_mic_btn,
        LV_STATE_DISABLED);

    bsp_display_unlock();
}
