/*
 * ============================================================
 * UI Events
 * ============================================================
 */

#include "ui_events.h"
#include "ui.h"
#include "mic.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static const char *TAG = "UI";

static bool recording = false;

/*----------------------------------------------------------*/

static void record_task(void *arg)
{
    ESP_LOGI(TAG, "Starting microphone...");

    if (mic_start() == ESP_OK)
    {
        ESP_LOGI(TAG, "Recording Finished");

        lv_label_set_text(
            ui_status,
            "Status : Recording Complete");

        if (mic_voice_detected())
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
    }

    lv_obj_set_style_bg_color(
        ui_mic_btn,
        lv_palette_main(LV_PALETTE_BLUE),
        0);

    recording = false;

    ESP_LOGI(TAG, "UI Updated");

    vTaskDelete(NULL);
}

/*----------------------------------------------------------*/

void mic_btn_event_cb(lv_event_t *e)
{
    if(recording)
        return;

    recording = true;

    ESP_LOGI(TAG, "Button Pressed");

    lv_obj_set_style_bg_color(
        ui_mic_btn,
        lv_palette_main(LV_PALETTE_RED),
        0);

    lv_label_set_text(
        ui_status,
        "Status : Recording...");

    lv_label_set_text(
        ui_speech_text,
        "Listening...");

    /* Force LCD refresh BEFORE recording */
    lv_refr_now(NULL);

    xTaskCreate(
        record_task,
        "record_task",
        8192,
        NULL,
        5,
        NULL);
}
