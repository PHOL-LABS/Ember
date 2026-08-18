#pragma once
#include <stdint.h>
#include "esp_err.h"

typedef struct {
    uint16_t rpm_signal;
    uint16_t speed_signal;
} ember_adc_sample_t;

esp_err_t ember_adc_init(void);
esp_err_t ember_adc_sample(ember_adc_sample_t *sample);
ember_adc_sample_t ember_adc_latest(void);
