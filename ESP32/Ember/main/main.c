#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "adc.h"
#include "ember_web.h"
#include "speedometer.h"
#include "tacho.h"

static void sampling_task(void *argument)
{
    (void)argument;
    while (true) {
        ESP_ERROR_CHECK_WITHOUT_ABORT(ember_adc_sample(NULL));
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    ESP_ERROR_CHECK(ember_adc_init());
    ESP_ERROR_CHECK(tacho_init());
    ESP_ERROR_CHECK(speedometer_init());
    xTaskCreate(sampling_task, "adc_sampling", 2048, NULL, 5, NULL);
    ESP_ERROR_CHECK(ember_web_start());
    ESP_LOGI("ember", "Ember is ready");
}
