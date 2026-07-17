/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Home Screen
 *
 * Compatible with:
 * ESP-IDF v5.5.4
 * LVGL v9.5
 * Waveshare BSP v2.0.0
 * ============================================================
 */

#include "ui_home.h"
#include "ui.h"

#include "lvgl.h"
#include "esp_log.h"

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

    lv_obj_remove_style_all(ui_screen);

    lv_obj_set_size(
        ui_screen,
        480,
        480);

    lv_obj_set_style_bg_color(
        ui_screen,
        lv_color_white(),
        0);

    lv_obj_set_style_bg_opa(
        ui_screen,
        LV_OPA_COVER,
        0);

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
        0);

    lv_obj_set_width(
        ui_title,
        440);

    lv_obj_set_style_text_align(
        ui_title,
        LV_TEXT_ALIGN_CENTER,
        0);

    lv_obj_align(
        ui_title,
        LV_ALIGN_TOP_MID,
        0,
        20);

    /*---------------------------------------------------------
     * Microphone Button
     *---------------------------------------------------------*/

    ui_mic_btn = lv_button_create(ui_screen);

    lv_obj_set_size(
        ui_mic_btn,
        240,
        70);

    lv_obj_align(
        ui_mic_btn,
        LV_ALIGN_TOP_MID,
        0,
        75);

    lv_obj_set_style_radius(
        ui_mic_btn,
        14,
        0);

    lv_obj_set_style_bg_color(
        ui_mic_btn,
        lv_palette_main(LV_PALETTE_BLUE),
        0);

    ui_mic_label = lv_label_create(ui_mic_btn);

    lv_label_set_text(
        ui_mic_label,
        "Press to Talk");

    lv_obj_set_style_text_font(
        ui_mic_label,
        &lv_font_montserrat_20,
        0);

    lv_obj_center(ui_mic_label);

    /*---------------------------------------------------------
     * Status
     *---------------------------------------------------------*/

    ui_status = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_status,
        "Status : Ready");

    lv_obj_set_width(
        ui_status,
        440);

    lv_obj_set_style_text_font(
        ui_status,
        &lv_font_montserrat_18,
        0);

    lv_obj_set_style_text_align(
        ui_status,
        LV_TEXT_ALIGN_CENTER,
        0);

    lv_obj_align(
        ui_status,
        LV_ALIGN_TOP_MID,
        0,
        175);

    /*---------------------------------------------------------
     * Speech Title
     *---------------------------------------------------------*/

    ui_speech_title = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_speech_title,
        "Recognized Speech");

    lv_obj_set_width(
        ui_speech_title,
        440);

    lv_obj_set_style_text_font(
        ui_speech_title,
        &lv_font_montserrat_18,
        0);

    lv_obj_set_style_text_align(
        ui_speech_title,
        LV_TEXT_ALIGN_CENTER,
        0);

    lv_obj_align(
        ui_speech_title,
        LV_ALIGN_TOP_MID,
        0,
        225);

    /*---------------------------------------------------------
     * Speech Text
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

    lv_obj_set_style_text_font(
        ui_speech_text,
        &lv_font_montserrat_18,
        0);

    lv_obj_set_style_text_align(
        ui_speech_text,
        LV_TEXT_ALIGN_CENTER,
        0);

    lv_obj_align(
        ui_speech_text,
        LV_ALIGN_TOP_MID,
        0,
        265);

    ESP_LOGI(TAG, "Home Screen Created");
}
