/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * UI Manager
 * Compatible with LVGL v9
 * ============================================================
 */

#include "ui.h"
#include "ui_home.h"

#include "esp_log.h"

static const char *TAG = "UI";

/*-------------------------------------------------------------
 * Global UI Objects
 *------------------------------------------------------------*/

/* Main Screen */
lv_obj_t *ui_screen = NULL;

/* Title */
lv_obj_t *ui_title = NULL;

/* Microphone Button */
lv_obj_t *ui_mic_btn = NULL;
lv_obj_t *ui_mic_label = NULL;

/* Status */
lv_obj_t *ui_status = NULL;

/* Speech */
lv_obj_t *ui_speech_title = NULL;
lv_obj_t *ui_speech_text = NULL;

/*-------------------------------------------------------------
 * UI Initialization
 *------------------------------------------------------------*/

void ui_init(void)
{
    ESP_LOGI(TAG, "Creating Home Screen");

    /* Build the complete UI */
    ui_home_create();

    /* Load the screen */
    lv_screen_load(ui_screen);

    ESP_LOGI(TAG, "UI Initialized Successfully");
}
