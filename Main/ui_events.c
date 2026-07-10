/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 5
 * UI Events
 * ============================================================
 */

#include "ui_events.h"
#include "ui.h"
#include "mic.h"

#include "esp_log.h"
#include "bsp/esp-bsp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "UI";

static void record_task(void *arg)
{
    ESP_LOGI(TAG, "Starting microphone...");

    if (mic_start() == ESP_OK)
    {
        ESP_LOGI(TAG, "Recording Finished");

        bsp_display_lock(portMAX_DELAY);

        lv_label_set_text(
            ui_status,
            "Status : Recording Complete");

        lv_label_set_text(
            ui_speech,
            "Recognized Speech:\n\n(Audio Captured)");

        lv_label_set_text(
            ui_response,
            "AI Response:\n\nReady for STT");

        lv_obj_set_style_bg_color(
            ui_mic_btn,
            lv_palette_main(LV_PALETTE_BLUE),
            0);

        bsp_display_unlock();

        ESP_LOGI(TAG, "UI Updated");
    }
    else
    {
        bsp_display_lock(portMAX_DELAY);

        lv_label_set_text(
            ui_status,
            "Status : Recording Failed");

        lv_obj_set_style_bg_color(
            ui_mic_btn,
            lv_palette_main(LV_PALETTE_BLUE),
            0);

        bsp_display_unlock();
    }

    vTaskDelete(NULL);
}

void mic_btn_event_cb(lv_event_t *e)
{
    ESP_LOGI(TAG, "MIC Button Pressed");

    bsp_display_lock(portMAX_DELAY);

    lv_obj_set_style_bg_color(
        ui_mic_btn,
        lv_palette_main(LV_PALETTE_RED),
        0);

    lv_label_set_text(
        ui_status,
        "Status : Recording...");

    lv_label_set_text(
        ui_speech,
        "Recognized Speech:\n\nListening...");

    lv_label_set_text(
        ui_response,
        "AI Response:\n\nWaiting for speech...");

    bsp_display_unlock();

    xTaskCreate(
        record_task,
        "record_task",
        8192,
        NULL,
        5,
        NULL);
}
