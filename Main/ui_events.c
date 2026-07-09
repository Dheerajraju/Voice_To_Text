/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 4.1
 * UI Events
 * ============================================================
 */

#include "ui_events.h"
#include "ui.h"

#include "esp_log.h"
#include "lvgl.h"

static const char *TAG = "UI";

/*-------------------------------------------------------------
 * Microphone Button Event
 *------------------------------------------------------------*/

void mic_btn_event_cb(lv_event_t *e)
{
    ESP_LOGI(TAG, "MIC Button Pressed");

    /* Change button color */

    lv_obj_set_style_bg_color(
        ui_mic_btn,
        lv_palette_main(LV_PALETTE_RED),
        0);

    /* Update status */

    lv_label_set_text(
        ui_status,
        "Status : Recording...");

    /* Update speech area */

    lv_label_set_text(
        ui_speech,
        "Recognized Speech:\n\nListening...");

    /* Update AI response */

    lv_label_set_text(
        ui_response,
        "AI Response:\n\nWaiting for speech...");

    ESP_LOGI(TAG, "UI Updated");
}
