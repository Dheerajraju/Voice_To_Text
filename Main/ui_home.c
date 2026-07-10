#include "ui_home.h"
#include "ui.h"
#include "ui_events.h"

#include "lvgl.h"

void ui_home_create(void)
{
    lv_obj_t *scr = lv_screen_active();

    /*--------------------------------------------------
     * Background
     *-------------------------------------------------*/
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x101820), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    /*--------------------------------------------------
     * Title
     *-------------------------------------------------*/
    lv_obj_t *title = lv_label_create(scr);

    lv_label_set_text(title, "AI Voice Assistant");

    lv_obj_set_style_text_color(title,
                                lv_color_white(),
                                LV_PART_MAIN);

    lv_obj_set_style_text_font(title,
                               &lv_font_montserrat_28,
                               LV_PART_MAIN);

    lv_obj_align(title,
                 LV_ALIGN_TOP_MID,
                 0,
                 40);

    /*--------------------------------------------------
     * Microphone Button
     *-------------------------------------------------*/
    ui_mic_btn = lv_button_create(scr);

    lv_obj_set_size(ui_mic_btn,
                    260,
                    80);

    lv_obj_align(ui_mic_btn,
                 LV_ALIGN_TOP_MID,
                 0,
                 120);

    lv_obj_set_style_radius(ui_mic_btn,
                            20,
                            LV_PART_MAIN);

    lv_obj_set_style_bg_color(ui_mic_btn,
                              lv_palette_main(LV_PALETTE_BLUE),
                              LV_PART_MAIN);

    lv_obj_add_event_cb(ui_mic_btn,
                        mic_btn_event_cb,
                        LV_EVENT_CLICKED,
                        NULL);

    lv_obj_t *btn_label = lv_label_create(ui_mic_btn);

    lv_label_set_text(btn_label,
                      "Press to Talk");

    lv_obj_set_style_text_color(btn_label,
                                lv_color_white(),
                                LV_PART_MAIN);

    lv_obj_set_style_text_font(btn_label,
                               &lv_font_montserrat_22,
                               LV_PART_MAIN);

    lv_obj_center(btn_label);

    /*--------------------------------------------------
     * Status
     *-------------------------------------------------*/
    ui_status = lv_label_create(scr);

    lv_label_set_text(ui_status,
                      "Status : Ready");

    lv_obj_set_style_text_color(ui_status,
                                lv_color_white(),
                                LV_PART_MAIN);

    lv_obj_set_style_text_font(ui_status,
                               &lv_font_montserrat_22,
                               LV_PART_MAIN);

    lv_obj_align(ui_status,
                 LV_ALIGN_TOP_MID,
                 0,
                 250);

    /*--------------------------------------------------
     * Speech Heading
     *-------------------------------------------------*/
    ui_speech = lv_label_create(scr);

    lv_label_set_text(ui_speech,
                      "Recognized Speech");

    lv_obj_set_style_text_color(ui_speech,
                                lv_color_white(),
                                LV_PART_MAIN);

    lv_obj_set_style_text_font(ui_speech,
                               &lv_font_montserrat_22,
                               LV_PART_MAIN);

    lv_obj_align(ui_speech,
                 LV_ALIGN_TOP_MID,
                 0,
                 340);

    /*--------------------------------------------------
     * AI Response
     *-------------------------------------------------*/
    ui_response = lv_label_create(scr);

    lv_label_set_text(ui_response,
                      "AI Response");

    lv_obj_set_style_text_color(ui_response,
                                lv_color_white(),
                                LV_PART_MAIN);

    lv_obj_set_style_text_font(ui_response,
                               &lv_font_montserrat_22,
                               LV_PART_MAIN);

    lv_obj_align(ui_response,
                 LV_ALIGN_TOP_MID,
                 0,
                 500);
}
