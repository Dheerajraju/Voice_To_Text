/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 4.1
 * Home Screen
 * ============================================================
 */

#include "ui_home.h"
#include "ui.h"
#include "ui_events.h"
#include "lvgl.h"

/*-------------------------------------------------------------
 * Create Home Screen
 *------------------------------------------------------------*/

void ui_home_create(void)
{
    /* Active Screen */

    ui_screen = lv_screen_active();

    /*---------------------------------------------------------
     * Background
     *--------------------------------------------------------*/

    lv_obj_set_style_bg_color(
        ui_screen,
        lv_color_hex(0x101820),
        LV_PART_MAIN);

    lv_obj_set_style_bg_opa(
        ui_screen,
        LV_OPA_COVER,
        LV_PART_MAIN);

    /*---------------------------------------------------------
     * Title
     *--------------------------------------------------------*/

    ui_title = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_title,
        "ESP32-P4 AI Voice Assistant");

    lv_obj_set_style_text_font(
        ui_title,
        &lv_font_montserrat_28,
        0);

    lv_obj_align(
        ui_title,
        LV_ALIGN_TOP_MID,
        0,
        25);

    /*---------------------------------------------------------
     * Microphone Button
     *--------------------------------------------------------*/

    ui_mic_btn = lv_button_create(ui_screen);

    lv_obj_set_size(
        ui_mic_btn,
        260,
        80);

    lv_obj_align(
        ui_mic_btn,
        LV_ALIGN_TOP_MID,
        0,
        90);

    lv_obj_set_style_radius(
        ui_mic_btn,
        20,
        0);

    lv_obj_set_style_bg_color(
        ui_mic_btn,
        lv_palette_main(LV_PALETTE_BLUE),
        0);

    lv_obj_add_event_cb(
        ui_mic_btn,
        mic_btn_event_cb,
        LV_EVENT_CLICKED,
        NULL);

    ui_mic_label = lv_label_create(ui_mic_btn);

    lv_label_set_text(
        ui_mic_label,
        "Press To Talk");

    lv_obj_set_style_text_font(
        ui_mic_label,
        &lv_font_montserrat_22,
        0);

    lv_obj_center(ui_mic_label);

    /*---------------------------------------------------------
     * Status Panel
     *--------------------------------------------------------*/

    ui_status = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_status,
        "Status : Ready");

    lv_obj_set_style_text_font(
        ui_status,
        &lv_font_montserrat_20,
        0);

    lv_obj_align(
        ui_status,
        LV_ALIGN_TOP_LEFT,
        40,
        220);

    /*---------------------------------------------------------
     * Speech Panel
     *--------------------------------------------------------*/

    ui_speech = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_speech,
        "Recognized Speech:\n\nWaiting...");

    lv_obj_set_width(
        ui_speech,
        640);

    lv_obj_set_style_text_font(
        ui_speech,
        &lv_font_montserrat_18,
        0);

    lv_obj_align(
        ui_speech,
        LV_ALIGN_TOP_LEFT,
        40,
        290);

    /*---------------------------------------------------------
     * AI Response Panel
     *--------------------------------------------------------*/

    ui_response = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_response,
        "AI Response:\n\nWaiting...");

    lv_obj_set_width(
        ui_response,
        640);

    lv_obj_set_style_text_font(
        ui_response,
        &lv_font_montserrat_18,
        0);

    lv_obj_align(
        ui_response,
        LV_ALIGN_TOP_LEFT,
        40,
        470);
}
