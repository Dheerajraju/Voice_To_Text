/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * UI Events
 *
 * Compatible with:
 * ESP-IDF v5.5.4
 * LVGL v9.5
 * Waveshare BSP v2.0.0
 * ============================================================
 */

#include "ui_events.h"
#include "ui.h"

#include "mic.h"
#include "speech.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static const char *TAG = "UI";

/*-------------------------------------------------------------
 * Speech Task
 *------------------------------------------------------------*/

static void speech_task(void *arg)
{
    char speech_text[512];

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Speech Task Started");
    ESP_LOGI(TAG, "====================================");

    ui_set_status("Status : Recording...");
    ui_set_speech("Listening...");

    /*---------------------------------------------------------
     * Record Audio
     *---------------------------------------------------------*/

    if (mic_start() != ESP_OK)
    {
        ui_set_status("Status : Mic Error");
        ui_set_speech("Recording Failed");

        ui_enable_button();

        vTaskDelete(NULL);
        return;
    }

    ui_set_status("Status : Recording Complete");

    /*---------------------------------------------------------
     * Voice Detection
     *---------------------------------------------------------*/

    if (!mic_voice_detected())
    {
        ui_set_status("Status : Ready");
        ui_set_speech("No Voice Detected");

        ui_enable_button();

        vTaskDelete(NULL);
        return;
    }

    ui_set_speech("Voice Detected");

    /*---------------------------------------------------------
     * Speech Recognition
     *---------------------------------------------------------*/

    ui_set_status("Status : Uploading...");
    ui_set_speech("Recognizing Speech...");

    if (speech_recognize(speech_text, sizeof(speech_text)) == ESP_OK)
    {
        ui_set_status("Status : Ready");
        ui_set_speech(speech_text);

        ESP_LOGI(TAG, "Speech Recognition Successful");
        ESP_LOGI(TAG, "%s", speech_text);
    }
    else
    {
        ui_set_status("Status : Server Error");
        ui_set_speech("Recognition Failed");

        ESP_LOGE(TAG, "Speech Recognition Failed");
    }

    /*---------------------------------------------------------
     * Enable Button Again
     *---------------------------------------------------------*/

    ui_enable_button();

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Speech Task Finished");
    ESP_LOGI(TAG, "====================================");

    vTaskDelete(NULL);
}

/*-------------------------------------------------------------
 * Button Event
 *------------------------------------------------------------*/

static void mic_button_event(lv_event_t *e)
{
    (void)e;

    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Button Pressed");
    ESP_LOGI(TAG, "====================================");

    ui_disable_button();

    BaseType_t ret = xTaskCreate(
        speech_task,
        "speech_task",
        8192,
        NULL,
        5,
        NULL);

    if (ret != pdPASS)
    {
        ui_enable_button();

        ui_set_status("Status : Error");
        ui_set_speech("Task Creation Failed");

        ESP_LOGE(TAG, "Failed to create speech task");
    }
}

/*-------------------------------------------------------------
 * Register Events
 *------------------------------------------------------------*/

void ui_events_init(void)
{
    lv_obj_add_event_cb(
        ui_mic_btn,
        mic_button_event,
        LV_EVENT_CLICKED,
        NULL);

    ESP_LOGI(TAG, "UI Events Initialized");
}
