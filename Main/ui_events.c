/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Stage 4.3
 * UI Events
 * Compatible with LVGL v9
 * ============================================================
 */

#include "ui_events.h"
#include "ui.h"
#include "mic.h"

#include "esp_log.h"

static const char *TAG = "UI";

static bool recording = false;

/*-------------------------------------------------------------
 * Microphone Button Event
 *------------------------------------------------------------*/

void mic_btn_event_cb(lv_event_t *e)
{
    LV_UNUSED(e);

    if(recording)
    {
        return;
    }

    recording = true;

    ESP_LOGI(TAG, "Button Pressed");

    /* Change button colour */
    lv_obj_set_style_bg_color(
        ui_mic_btn,
        lv_palette_main(LV_PALETTE_RED),
        0);

    /* Update status */
    lv_label_set_text(
        ui_status,
        "Status : Recording...");

    lv_label_set_text(
        ui_speech_text,
        "Listening...");

    /* Force immediate redraw */
    lv_refr_now(NULL);

    ESP_LOGI(TAG, "Starting microphone...");

    if(mic_start() == ESP_OK)
    {
        ESP_LOGI(TAG, "Recording Finished");

        lv_label_set_text(
            ui_status,
            "Status : Recording Complete");

        if(mic_voice_detected())
        {
            lv_label_set_text(
                ui_speech_text,
                "Voice Detected");
        }
        else
        {
            lv_label_set_text(
                ui_speech_text,
                "No Voice Detected");
        }
    }
    else
    {
        lv_label_set_text(
            ui_status,
            "Status : Recording Failed");

        lv_label_set_text(
            ui_speech_text,
            "");
    }

    /* Restore button colour */
    lv_obj_set_style_bg_color(
        ui_mic_btn,
        lv_palette_main(LV_PALETTE_BLUE),
        0);

    lv_refr_now(NULL);

    ESP_LOGI(TAG, "UI Updated");

    recording = false;
}
