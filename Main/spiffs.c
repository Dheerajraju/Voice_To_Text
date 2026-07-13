/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * Stage 4
 * SPIFFS Filesystem
 * ============================================================
 */

#include "spiffs.h"

#include <stdio.h>

#include "esp_log.h"
#include "esp_spiffs.h"

static const char *TAG = "SPIFFS";

/*-------------------------------------------------------------
 * Initialize SPIFFS
 *------------------------------------------------------------*/

esp_err_t spiffs_init(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS...");

    esp_vfs_spiffs_conf_t conf =
    {
        .base_path = "/spiffs",
        .partition_label = "storage",
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to mount SPIFFS (%s)",
                 esp_err_to_name(ret));
        return ret;
    }

    size_t total = 0;
    size_t used = 0;

    ret = esp_spiffs_info(conf.partition_label,
                          &total,
                          &used);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG,
                 "Failed to get SPIFFS info (%s)",
                 esp_err_to_name(ret));

        return ret;
    }

    ESP_LOGI(TAG, "================================");
    ESP_LOGI(TAG, "SPIFFS Mounted Successfully");
    ESP_LOGI(TAG, "Mount Point : %s", conf.base_path);
    ESP_LOGI(TAG, "Partition   : %s", conf.partition_label);
    ESP_LOGI(TAG, "Total Space : %u bytes",
             (unsigned)total);
    ESP_LOGI(TAG, "Used Space  : %u bytes",
             (unsigned)used);
    ESP_LOGI(TAG, "Free Space  : %u bytes",
             (unsigned)(total - used));
    ESP_LOGI(TAG, "================================");

    return ESP_OK;
}

/*-------------------------------------------------------------
 * Print SPIFFS Info
 *------------------------------------------------------------*/

void spiffs_print_info(void)
{
    size_t total = 0;
    size_t used = 0;

    if (esp_spiffs_info("storage",
                        &total,
                        &used) == ESP_OK)
    {
        ESP_LOGI(TAG,
                 "SPIFFS Total : %u bytes",
                 (unsigned)total);

        ESP_LOGI(TAG,
                 "SPIFFS Used  : %u bytes",
                 (unsigned)used);

        ESP_LOGI(TAG,
                 "SPIFFS Free  : %u bytes",
                 (unsigned)(total - used));
    }
}
