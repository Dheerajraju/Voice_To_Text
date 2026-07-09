#include "ui.h"
#include "ui_events.h"

static lv_obj_t *status_label;
static lv_obj_t *indicator;

void ui_set_status(const char *text)
{
    if (status_label)
        lv_label_set_text(status_label, text);
}

void ui_set_indicator_color(lv_color_t color)
{
    if (indicator)
        lv_obj_set_style_bg_color(indicator, color, 0);
}

void ui_create(void)
{
    lv_obj_t *scr = lv_screen_active();

    lv_obj_set_style_bg_color(scr, lv_color_white(), 0);

    /* Title */

    lv_obj_t *title = lv_label_create(scr);

    lv_label_set_text(title, "VOICE ASSISTANT");

    lv_obj_set_style_text_font(title,
                               &lv_font_montserrat_28,
                               0);

    lv_obj_align(title,
                 LV_ALIGN_TOP_MID,
                 0,
                 25);

    /* Button */

    lv_obj_t *btn = lv_button_create(scr);

    lv_obj_set_size(btn, 320, 90);

    lv_obj_align(btn,
                 LV_ALIGN_CENTER,
                 0,
                 -30);

    lv_obj_add_event_cb(btn,
                        mic_btn_event_cb,
                        LV_EVENT_CLICKED,
                        NULL);

    lv_obj_t *btn_label = lv_label_create(btn);

    lv_label_set_text(btn_label,
                      "PRESS TO TALK");

    lv_obj_set_style_text_font(btn_label,
                               &lv_font_montserrat_24,
                               0);

    lv_obj_center(btn_label);

    /* Status */

    status_label = lv_label_create(scr);

    lv_label_set_text(status_label,
                      "STATUS : READY");

    lv_obj_set_style_text_font(status_label,
                               &lv_font_montserrat_22,
                               0);

    lv_obj_align(status_label,
                 LV_ALIGN_CENTER,
                 0,
                 70);

    /* Indicator */

    indicator = lv_obj_create(scr);

    lv_obj_remove_style_all(indicator);

    lv_obj_set_size(indicator, 22, 22);

    lv_obj_set_style_radius(indicator,
                            LV_RADIUS_CIRCLE,
                            0);

    lv_obj_set_style_bg_color(indicator,
                              lv_palette_main(LV_PALETTE_GREY),
                              0);

    lv_obj_align(indicator,
                 LV_ALIGN_CENTER,
                 0,
                 120);
}
