/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Stage 4
 * Home Screen
 * ============================================================
 */

#include "ui_home.h"
#include "ui.h"

#include "esp_log.h"
#include "lvgl.h"

static const char *TAG = "UI";

/*-------------------------------------------------------------
 * Create Home Screen
 *------------------------------------------------------------*/

void ui_home_create(void)
{
    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Creating Home Screen");
    ESP_LOGI(TAG, "====================================");

    /*---------------------------------------------------------
     * Screen
     *---------------------------------------------------------*/

    ui_screen = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(
    ui_screen,
    lv_color_white(),
    LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_opa(
        ui_screen,
        LV_OPA_COVER,
        LV_PART_MAIN);

    /*---------------------------------------------------------
     * Title
     *---------------------------------------------------------*/

    ui_title = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_title,
        "AI Voice Assistant");

    lv_obj_set_style_text_font(
        ui_title,
        &lv_font_montserrat_24,
        LV_PART_MAIN);

    lv_obj_align(
        ui_title,
        LV_ALIGN_TOP_MID,
        0,
        25);

    /*---------------------------------------------------------
     * Microphone Button
     *---------------------------------------------------------*/

    ui_mic_btn = lv_button_create(ui_screen);

    lv_obj_set_size(
        ui_mic_btn,
        220,
        70);

    lv_obj_align(
        ui_mic_btn,
        LV_ALIGN_TOP_MID,
        0,
        90);

    lv_obj_set_style_bg_color(
        ui_mic_btn,
        lv_palette_main(LV_PALETTE_BLUE),
        LV_PART_MAIN);

    lv_obj_set_style_radius(
        ui_mic_btn,
        12,
        LV_PART_MAIN);

    ui_mic_label = lv_label_create(ui_mic_btn);

    lv_label_set_text(
        ui_mic_label,
        "Press to talk");

    lv_obj_center(ui_mic_label);

    /*---------------------------------------------------------
     * Status Label
     *---------------------------------------------------------*/

    ui_status = lv_label_create(ui_screen);

    lv_label_set_text(
    ui_status,
    "Ready");

    lv_obj_set_style_text_font(
        ui_status,
        &lv_font_montserrat_18,
        LV_PART_MAIN);

    lv_obj_align(
        ui_status,
        LV_ALIGN_TOP_LEFT,
        30,
        190);

    /*---------------------------------------------------------
     * Speech Title
     *---------------------------------------------------------*/

    ui_speech_title = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_speech_title,
        "Recognized Speech");

    lv_obj_set_style_text_font(
        ui_speech_title,
        &lv_font_montserrat_18,
        LV_PART_MAIN);

    lv_obj_align(
        ui_speech_title,
        LV_ALIGN_TOP_LEFT,
        30,
        235);

    /*---------------------------------------------------------
     * Speech Result
     *---------------------------------------------------------*/

    ui_speech_text = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_speech_text,
        "");

    lv_obj_set_width(
        ui_speech_text,
        420);

    lv_label_set_long_mode(
        ui_speech_text,
        LV_LABEL_LONG_WRAP);

    lv_obj_align(
        ui_speech_text,
        LV_ALIGN_TOP_LEFT,
        30,
        270);

    /*---------------------------------------------------------
     * IMPORTANT
     *---------------------------------------------------------
     * DO NOT register button events here.
     *
     * ui_events_init() performs:
     *
     * lv_obj_add_event_cb(
     *      ui_mic_btn,
     *      mic_button_event,
     *      LV_EVENT_CLICKED,
     *      NULL);
     *
     * This prevents duplicate callbacks and linker errors.
     *---------------------------------------------------------*/

    ESP_LOGI(TAG, "Home Screen Created");
}
