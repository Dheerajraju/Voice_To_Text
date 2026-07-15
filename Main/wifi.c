/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * WiFi Manager
 * Compatible with ESP-IDF v5.5.4
 * ============================================================
 */

#include "wifi.h"

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"

static const char *TAG = "WIFI";

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define MAXIMUM_RETRY      10

static EventGroupHandle_t wifi_event_group = NULL;
static int retry_count = 0;

/*-------------------------------------------------------------
 * WiFi Event Handler
 *------------------------------------------------------------*/

static void wifi_event_handler(
    void *arg,
    esp_event_base_t event_base,
    int32_t event_id,
    void *event_data)
{
    if (event_base == WIFI_EVENT)
    {
        switch (event_id)
        {
            case WIFI_EVENT_STA_START:

                ESP_LOGI(TAG,
                         "Connecting to \"%s\"...",
                         WIFI_SSID);

                esp_wifi_connect();

                break;

            case WIFI_EVENT_STA_DISCONNECTED:
            {
                wifi_event_sta_disconnected_t *event =
                    (wifi_event_sta_disconnected_t *)event_data;

                ESP_LOGW(TAG,
                         "Disconnected (Reason=%d)",
                         event->reason);

                if (retry_count < MAXIMUM_RETRY)
                {
                    retry_count++;

                    ESP_LOGI(TAG,
                             "Reconnect attempt %d/%d",
                             retry_count,
                             MAXIMUM_RETRY);

                    esp_wifi_connect();
                }
                else
                {
                    ESP_LOGE(TAG,
                             "Maximum retry reached.");

                    xEventGroupSetBits(
                        wifi_event_group,
                        WIFI_FAIL_BIT);
                }

                break;
            }

            default:
                break;
        }
    }

    if ((event_base == IP_EVENT) &&
        (event_id == IP_EVENT_STA_GOT_IP))
    {
        ip_event_got_ip_t *event =
            (ip_event_got_ip_t *)event_data;

        retry_count = 0;

        ESP_LOGI(TAG,
                 "===================================");

        ESP_LOGI(TAG,
                 "WiFi Connected");

        ESP_LOGI(TAG,
                 "SSID : %s",
                 WIFI_SSID);

        ESP_LOGI(TAG,
                 "IP Address : " IPSTR,
                 IP2STR(&event->ip_info.ip));

        ESP_LOGI(TAG,
                 "Gateway    : " IPSTR,
                 IP2STR(&event->ip_info.gw));

        ESP_LOGI(TAG,
                 "Netmask    : " IPSTR,
                 IP2STR(&event->ip_info.netmask));

        ESP_LOGI(TAG,
                 "===================================");

        xEventGroupSetBits(
            wifi_event_group,
            WIFI_CONNECTED_BIT);
    }
}

/*-------------------------------------------------------------
 * Initialize WiFi
 *------------------------------------------------------------*/

esp_err_t wifi_init_sta(void)
{
    esp_err_t ret;

    ESP_LOGI(TAG,
             "===================================");

    ESP_LOGI(TAG,
             "Initializing WiFi");

    ESP_LOGI(TAG,
             "===================================");

    retry_count = 0;

    if (wifi_event_group == NULL)
    {
        wifi_event_group = xEventGroupCreate();

        if (wifi_event_group == NULL)
        {
            ESP_LOGE(TAG,
                     "Failed to create EventGroup");

            return ESP_ERR_NO_MEM;
        }
    }

    /*---------------------------------------------------------
     * TCP/IP Stack
     *---------------------------------------------------------*/

    ret = esp_netif_init();

    if ((ret != ESP_OK) &&
        (ret != ESP_ERR_INVALID_STATE))
    {
        ESP_LOGE(TAG,
                 "esp_netif_init() failed");

        return ret;
    }

    /*---------------------------------------------------------
     * Event Loop
     *---------------------------------------------------------*/

    ret = esp_event_loop_create_default();

    if ((ret != ESP_OK) &&
        (ret != ESP_ERR_INVALID_STATE))
    {
        ESP_LOGE(TAG,
                 "Event loop creation failed");

        return ret;
    }

    /*---------------------------------------------------------
     * Default Station Interface
     *---------------------------------------------------------*/

    esp_netif_t *sta_netif =
        esp_netif_create_default_wifi_sta();

    if (sta_netif == NULL)
    {
        ESP_LOGW(TAG,
                 "Default STA already exists");
    }

    /*---------------------------------------------------------
     * WiFi Driver
     *---------------------------------------------------------*/

    wifi_init_config_t cfg =
        WIFI_INIT_CONFIG_DEFAULT();

    ret = esp_wifi_init(&cfg);

    if ((ret != ESP_OK) &&
        (ret != ESP_ERR_INVALID_STATE))
    {
        ESP_LOGE(TAG,
                 "esp_wifi_init() failed");

        return ret;
    }

    /*---------------------------------------------------------
     * Register Events
     *---------------------------------------------------------*/

    ESP_ERROR_CHECK(
        esp_event_handler_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &wifi_event_handler,
            NULL));

    ESP_ERROR_CHECK(
        esp_event_handler_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &wifi_event_handler,
            NULL));

    /*---------------------------------------------------------
     * WiFi Configuration
     *---------------------------------------------------------*/

    wifi_config_t wifi_config = {0};

    strncpy(
        (char *)wifi_config.sta.ssid,
        WIFI_SSID,
        sizeof(wifi_config.sta.ssid));

    strncpy(
        (char *)wifi_config.sta.password,
        WIFI_PASSWORD,
        sizeof(wifi_config.sta.password));

    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;

    /*---------------------------------------------------------
     * Configure WiFi
     *---------------------------------------------------------*/

    ESP_ERROR_CHECK(
        esp_wifi_set_mode(WIFI_MODE_STA));

    ESP_ERROR_CHECK(
        esp_wifi_set_config(
            WIFI_IF_STA,
            &wifi_config));

    /*---------------------------------------------------------
     * Restart if already running
     *---------------------------------------------------------*/

    esp_wifi_disconnect();

    ret = esp_wifi_start();

    if ((ret != ESP_OK) &&
        (ret != ESP_ERR_WIFI_CONN))
    {
        ESP_LOGE(TAG,
                 "esp_wifi_start() failed");

        return ret;
    }

    /*---------------------------------------------------------
     * Wait for Connection
     *---------------------------------------------------------*/

    EventBits_t bits =
        xEventGroupWaitBits(
            wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            pdMS_TO_TICKS(20000));

    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG,
                 "WiFi Ready");

        return ESP_OK;
    }

    if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGE(TAG,
                 "Failed to connect to AP");

        return ESP_FAIL;
    }

    ESP_LOGE(TAG,
             "Connection timed out");

    return ESP_ERR_TIMEOUT;
}
