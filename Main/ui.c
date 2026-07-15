/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * UI Manager
 * ============================================================
 */

#include "ui.h"
#include "ui_home.h"

#include "esp_log.h"

static const char *TAG = "UI";


lv_obj_t *ui_screen = NULL;

lv_obj_t *ui_title = NULL;

lv_obj_t *ui_mic_btn = NULL;
lv_obj_t *ui_mic_label = NULL;

lv_obj_t *ui_status = NULL;

lv_obj_t *ui_speech_title = NULL;
lv_obj_t *ui_speech_text = NULL;



void ui_init(void)
{
    ESP_LOGI(TAG,
             "====================================");

    ESP_LOGI(TAG,
             "Creating Home Screen");


    ui_home_create();


    if(ui_screen != NULL)
    {
        lv_screen_load(ui_screen);
    }


    ESP_LOGI(TAG,
             "UI Initialized Successfully");
}



void ui_set_status(const char *text)
{
    if(ui_status == NULL)
    {
        return;
    }


    lv_label_set_text(
        ui_status,
        text);
}



void ui_set_speech(const char *text)
{
    if(ui_speech_text == NULL)
    {
        return;
    }


    lv_label_set_text(
        ui_speech_text,
        text);
}
