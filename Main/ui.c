/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Stage 4.3
 * UI Manager
 * ============================================================
 */

#include "ui.h"
#include "ui_home.h"

#include "esp_log.h"

static const char *TAG = "UI";

/*-------------------------------------------------------------
 * Global UI Objects
 *------------------------------------------------------------*/

lv_obj_t *ui_screen = NULL;

lv_obj_t *ui_title = NULL;

lv_obj_t *ui_mic_btn = NULL;
lv_obj_t *ui_mic_label = NULL;

lv_obj_t *ui_status = NULL;

lv_obj_t *ui_speech_title = NULL;
lv_obj_t *ui_speech_text = NULL;

/*-------------------------------------------------------------
 * Initialize UI
 *------------------------------------------------------------*/

void ui_init(void)
{
    ESP_LOGI(TAG, "====================================");
    ESP_LOGI(TAG, "Creating Home Screen");
    ESP_LOGI(TAG, "====================================");

    /* Create complete home screen */
    ui_home_create();

    /* Make it the active screen */
    lv_screen_load(ui_screen);

    /* Force immediate refresh */
    lv_refr_now(NULL);

    ESP_LOGI(TAG, "UI Initialized Successfully");
}
