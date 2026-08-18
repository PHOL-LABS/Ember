#pragma once
#include <stdint.h>
#include "driver/gpio.h"
#include "esp_err.h"

typedef struct {
    gpio_num_t gpio;
    volatile int64_t last_edge_us;
    volatile uint32_t period_us;
} frequency_input_t;

esp_err_t frequency_count_init(frequency_input_t *input, gpio_num_t gpio);
float frequency_count_hz(const frequency_input_t *input);
