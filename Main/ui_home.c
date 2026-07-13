/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Home Screen
 * Optimized for 720x720 Display
 * ============================================================
 */

#include "ui_home.h"
#include "ui_events.h"

#include "lvgl.h"

void ui_home_create(void)
{
    /*---------------------------------------------------------
     * Screen
     *---------------------------------------------------------*/
    ui_screen = lv_obj_create(NULL);

    lv_obj_remove_style_all(ui_screen);

    lv_obj_set_size(ui_screen, 720, 720);

    lv_obj_set_style_bg_color(
        ui_screen,
        lv_color_white(),
        LV_PART_MAIN);

    /*---------------------------------------------------------
     * Title
     *---------------------------------------------------------*/

    ui_title = lv_label_create(ui_screen);

    lv_label_set_text(ui_title, "AI Voice Assistant");

    lv_obj_set_style_text_font(
        ui_title,
        &lv_font_montserrat_30,
        0);

    lv_obj_align(
        ui_title,
        LV_ALIGN_TOP_MID,
        0,
        45);

    /*---------------------------------------------------------
     * Button
     *---------------------------------------------------------*/

    ui_mic_btn = lv_button_create(ui_screen);

    lv_obj_set_size(
        ui_mic_btn,
        300,
        90);

    lv_obj_align(
        ui_mic_btn,
        LV_ALIGN_TOP_MID,
        0,
        120);

    lv_obj_set_style_radius(
        ui_mic_btn,
        18,
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
        "Press to Talk");

    lv_obj_set_style_text_font(
        ui_mic_label,
        &lv_font_montserrat_22,
        0);

    lv_obj_set_style_text_color(
        ui_mic_label,
        lv_color_white(),
        0);

    lv_obj_center(ui_mic_label);

    /*---------------------------------------------------------
     * Status
     *---------------------------------------------------------*/

    ui_status = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_status,
        "Status : Ready");

    lv_obj_set_width(ui_status, 600);

    lv_obj_set_style_text_align(
        ui_status,
        LV_TEXT_ALIGN_CENTER,
        0);

    lv_obj_set_style_text_font(
        ui_status,
        &lv_font_montserrat_22,
        0);

    lv_obj_align(
        ui_status,
        LV_ALIGN_TOP_MID,
        0,
        255);

    /*---------------------------------------------------------
     * Speech Title
     *---------------------------------------------------------*/

    ui_speech_title = lv_label_create(ui_screen);

    lv_label_set_text(
        ui_speech_title,
        "Recognized Speech");

    lv_obj_set_style_text_font(
        ui_speech_title,
        &lv_font_montserrat_24,
        0);

    lv_obj_align(
        ui_speech_title,
        LV_ALIGN_TOP_MID,
        0,
        330);

    /*---------------------------------------------------------
     * Speech Text
     *---------------------------------------------------------*/

    ui_speech_text = lv_label_create(ui_screen);

    lv_obj_set_width(
        ui_speech_text,
        620);

    lv_label_set_long_mode(
        ui_speech_text,
        LV_LABEL_LONG_WRAP);

    lv_obj_set_style_text_align(
        ui_speech_text,
        LV_TEXT_ALIGN_CENTER,
        0);

    lv_obj_set_style_text_font(
        ui_speech_text,
        &lv_font_montserrat_22,
        0);

    lv_label_set_text(
        ui_speech_text,
        "");

    lv_obj_align(
        ui_speech_text,
        LV_ALIGN_TOP_MID,
        0,
        385);
}
