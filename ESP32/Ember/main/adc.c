#include "adc.h"
#include "esp_adc/adc_oneshot.h"

#define RPM_ADC_CHANNEL ADC_CHANNEL_6   /* GPIO34 on ESP32 */
#define SPEED_ADC_CHANNEL ADC_CHANNEL_7 /* GPIO35 on ESP32 */

static adc_oneshot_unit_handle_t unit;
static ember_adc_sample_t latest;

esp_err_t ember_adc_init(void)
{
    const adc_oneshot_unit_init_cfg_t unit_config = {.unit_id = ADC_UNIT_1};
    const adc_oneshot_chan_cfg_t channel_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    esp_err_t err = adc_oneshot_new_unit(&unit_config, &unit);
    if (err != ESP_OK) return err;
    err = adc_oneshot_config_channel(unit, RPM_ADC_CHANNEL, &channel_config);
    if (err != ESP_OK) return err;
    return adc_oneshot_config_channel(unit, SPEED_ADC_CHANNEL, &channel_config);
}

esp_err_t ember_adc_sample(ember_adc_sample_t *sample)
{
    int rpm = 0, speed = 0;
    esp_err_t err = adc_oneshot_read(unit, RPM_ADC_CHANNEL, &rpm);
    if (err != ESP_OK) return err;
    err = adc_oneshot_read(unit, SPEED_ADC_CHANNEL, &speed);
    if (err != ESP_OK) return err;
    latest.rpm_signal = (uint16_t)rpm;
    latest.speed_signal = (uint16_t)speed;
    if (sample) *sample = latest;
    return ESP_OK;
}

ember_adc_sample_t ember_adc_latest(void) { return latest; }
