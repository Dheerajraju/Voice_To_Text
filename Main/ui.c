/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 4.1
 * UI Implementation
 * ============================================================
 */

#include "ui.h"
#include "ui_home.h"

/*-------------------------------------------------------------
 * Global UI Objects
 *------------------------------------------------------------*/

lv_obj_t *ui_screen = NULL;

lv_obj_t *ui_title = NULL;

lv_obj_t *ui_mic_btn = NULL;
lv_obj_t *ui_mic_label = NULL;

lv_obj_t *ui_status = NULL;

lv_obj_t *ui_speech = NULL;

lv_obj_t *ui_response = NULL;

/*-------------------------------------------------------------
 * UI Initialization
 *------------------------------------------------------------*/

void ui_init(void)
{
    ui_home_create();
}
