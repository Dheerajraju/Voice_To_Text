/*
 * ============================================================
 * ESP32-P4 AI Voice Assistant
 * VTT_V8
 * Microphone Driver
 * ============================================================
 */

#include "mic.h"

#include <string.h>
#include <math.h>

#include "esp_log.h"
#include "esp_heap_caps.h"

#include "esp_codec_dev.h"
#include "esp_codec_dev_types.h"

#include "bsp/esp-bsp.h"

#include "wav.h"


static const char *TAG = "MIC";


/*-------------------------------------------------------------
 * Private Variables
 *------------------------------------------------------------*/

static esp_codec_dev_handle_t mic_codec = NULL;


/*
 * IMPORTANT:
 * This must be a pointer because memory is allocated
 * dynamically from PSRAM.
 */
/*
 * Audio buffer
 * Allocated in PSRAM during mic_init()
 */
static int16_t *audio_buffer = NULL;


static uint32_t sample_count = 0;

static float rms_value = 0.0f;

static int16_t peak_value = 0;

static bool voice_detected = false;



/*-------------------------------------------------------------
 * Analyze Audio
 *------------------------------------------------------------*/

static void analyze_audio(void)
{
    int64_t sum = 0;

    peak_value = 0;


    for(uint32_t i = 0; i < sample_count; i++)
    {
        int16_t sample = audio_buffer[i];


        if(abs(sample) > peak_value)
        {
            peak_value = abs(sample);
        }


        sum += ((int32_t)sample * sample);
    }


    if(sample_count > 0)
    {
        rms_value = sqrt((double)sum / sample_count);
    }
    else
    {
        rms_value = 0;
    }


    voice_detected =
        (rms_value >= MIC_RMS_THRESHOLD);



    ESP_LOGI(TAG,"========== AUDIO ANALYSIS ==========");
    ESP_LOGI(TAG,"Samples        : %lu",
             (unsigned long)sample_count);

    ESP_LOGI(TAG,"Peak           : %d",
             peak_value);

    ESP_LOGI(TAG,"RMS            : %.2f",
             rms_value);

    ESP_LOGI(TAG,"Voice Detected : %s",
             voice_detected ? "YES":"NO");

    ESP_LOGI(TAG,"====================================");
}



/*-------------------------------------------------------------
 * Initialize Microphone
 *------------------------------------------------------------*/

esp_err_t mic_init(void)
{
    ESP_LOGI(TAG,"Initializing microphone...");



    audio_buffer = (int16_t *)heap_caps_malloc(
        AUDIO_BUFFER_SIZE,
        MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    if (audio_buffer == NULL)
    {
        ESP_LOGE(TAG, "Failed allocating audio buffer");
        return ESP_ERR_NO_MEM;
    }

    memset(audio_buffer, 0, AUDIO_BUFFER_SIZE);



    esp_err_t ret;


    ret = bsp_audio_init(NULL);


    if(ret != ESP_OK)
    {
        ESP_LOGE(TAG,
                 "bsp_audio_init failed");

        return ret;
    }



    mic_codec =
        bsp_audio_codec_microphone_init();



    if(mic_codec == NULL)
    {
        ESP_LOGE(TAG,
                 "Microphone codec failed");

        return ESP_FAIL;
    }



    esp_codec_dev_sample_info_t fs =
    {
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = 16,
        .channel = 1,
        .channel_mask = 1,
        .mclk_multiple = 256
    };



    ret =
    esp_codec_dev_open(
        mic_codec,
        &fs);



    if(ret != ESP_CODEC_DEV_OK)
    {
        ESP_LOGE(TAG,
                 "Codec open failed");

        return ESP_FAIL;
    }



    esp_codec_dev_set_in_gain(
        mic_codec,
        30.0f);



    ESP_LOGI(TAG,
             "Microphone initialized successfully");


    return ESP_OK;
}




/*-------------------------------------------------------------
 * Start Recording
 *------------------------------------------------------------*/

esp_err_t mic_start(void)
{
    esp_err_t ret;


    if(mic_codec == NULL)
    {
        ESP_LOGE(TAG,
                 "Microphone not initialized");

        return ESP_FAIL;
    }



    memset(
        audio_buffer,
        0,
        AUDIO_BUFFER_SIZE);



    sample_count = 0;



    ESP_LOGI(TAG,
             "Recording...");



    while(sample_count < AUDIO_BUFFER_SAMPLES)
    {

        uint32_t remaining =
            AUDIO_BUFFER_SAMPLES -
            sample_count;



        uint32_t block =
            (remaining > 1024)
            ?1024
            :remaining;



        ret =
        esp_codec_dev_read(
            mic_codec,
            &audio_buffer[sample_count],
            block * sizeof(int16_t));



        if(ret != ESP_CODEC_DEV_OK)
        {
            ESP_LOGE(TAG,
                     "Audio read failed");

            return ESP_FAIL;
        }



        sample_count += block;

    }



    ESP_LOGI(TAG,
             "Captured %lu samples",
             (unsigned long)sample_count);



    analyze_audio();



    ESP_LOGI(TAG,
             "Saving WAV file...");



    ret = wav_save_file(
            "/spiffs/record.wav",
            audio_buffer,
            sample_count);



    if(ret != ESP_OK)
    {
        ESP_LOGE(TAG,
                 "Failed to save WAV");

        return ret;
    }



    ESP_LOGI(TAG,
             "WAV file saved successfully");


    return ESP_OK;
}





/*-------------------------------------------------------------
 * Stop
 *------------------------------------------------------------*/

esp_err_t mic_stop(void)
{

    if(mic_codec)
    {
        esp_codec_dev_close(mic_codec);
    }


    ESP_LOGI(TAG,
             "Recording stopped");


    return ESP_OK;
}



/*-------------------------------------------------------------
 * Get Buffer
 *------------------------------------------------------------*/

int16_t *mic_get_buffer(void)
{
    return audio_buffer;
}




uint32_t mic_get_sample_count(void)
{
    return sample_count;
}




float mic_get_rms(void)
{
    return rms_value;
}




int16_t mic_get_peak(void)
{
    return peak_value;
}




bool mic_voice_detected(void)
{
    return voice_detected;
}


