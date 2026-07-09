#include "ui_events.h"
#include "ui.h"

#include "esp_log.h"

static const char *TAG = "UI";

static lv_timer_t *record_timer = NULL;
static lv_timer_t *blink_timer = NULL;

static bool visible = true;

static void blink_cb(lv_timer_t *t)
{
    visible = !visible;

    if (visible)
    {
        ui_set_indicator_color(lv_palette_main(LV_PALETTE_RED));
    }
    else
    {
        ui_set_indicator_color(lv_color_white());
    }
}

static void record_finished(lv_timer_t *t)
{
    if (blink_timer)
    {
        lv_timer_del(blink_timer);
        blink_timer = NULL;
    }

    ui_set_status("STATUS : COMPLETED");

    ui_set_indicator_color(
        lv_palette_main(LV_PALETTE_GREEN));

    record_timer = NULL;
}

void mic_btn_event_cb(lv_event_t *e)
{
    if (record_timer != NULL)
        return;

    ESP_LOGI(TAG, "Recording Started");

    ui_set_status("STATUS : RECORDING");

    ui_set_indicator_color(
        lv_palette_main(LV_PALETTE_RED));

    visible = true;

    blink_timer = lv_timer_create(
        blink_cb,
        500,
        NULL);

    record_timer = lv_timer_create(
        record_finished,
        5000,
        NULL);

    lv_timer_set_repeat_count(
        record_timer,
        1);
}
