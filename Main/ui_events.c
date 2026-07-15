/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * UI Events
 * ============================================================
 */


#include "ui_events.h"

#include "ui.h"
#include "mic.h"
#include "speech.h"

#include "esp_log.h"


static const char *TAG = "UI";


static void mic_button_event(lv_event_t *e)
{

    ESP_LOGI(TAG,
             "MIC BUTTON PRESSED");


    /*
     * Temporary UI update only.
     *
     * Heavy operations will be moved
     * into a FreeRTOS task.
     */


    ui_set_status(
        "Starting recording...");


    ui_set_speech(
        "Listening...");



    esp_err_t ret =
        mic_start();



    if(ret != ESP_OK)
    {

        ESP_LOGE(TAG,
                 "Recording failed");


        ui_set_status(
            "Recording Error");


        ui_set_speech(
            "Mic failed");


        return;
    }



    if(!mic_voice_detected())
    {

        ESP_LOGW(TAG,
                 "No voice detected");


        ui_set_status(
            "Ready");


        ui_set_speech(
            "No voice detected");


        return;
    }



    ui_set_status(
        "Recognizing...");



    char text[512];


    ret =
        speech_recognize(
            text,
            sizeof(text));



    if(ret == ESP_OK)
    {

        ui_set_status(
            "Complete");


        ui_set_speech(
            text);


        ESP_LOGI(TAG,
                 "Speech:");
        ESP_LOGI(TAG,
                 "%s",
                 text);

    }
    else
    {

        ui_set_status(
            "Server Error");


        ui_set_speech(
            "Recognition failed");

    }


}



void ui_events_init(void)
{

    if(ui_mic_btn == NULL)
    {

        ESP_LOGE(TAG,
                 "MIC button NULL");

        return;
    }



    lv_obj_add_event_cb(
        ui_mic_btn,
        mic_button_event,
        LV_EVENT_CLICKED,
        NULL);



    ESP_LOGI(TAG,
             "UI Events Initialized");

}
