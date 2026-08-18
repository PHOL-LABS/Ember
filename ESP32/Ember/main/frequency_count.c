#include "frequency_count.h"
#include "esp_attr.h"
#include "esp_timer.h"

static void IRAM_ATTR edge_handler(void *argument)
{
    frequency_input_t *input = argument;
    int64_t now = esp_timer_get_time();
    int64_t elapsed = now - input->last_edge_us;
    if (input->last_edge_us && elapsed >= 100) input->period_us = (uint32_t)elapsed;
    input->last_edge_us = now;
}

esp_err_t frequency_count_init(frequency_input_t *input, gpio_num_t gpio)
{
    input->gpio = gpio;
    input->last_edge_us = 0;
    input->period_us = 0;
    gpio_config_t config = {
        .pin_bit_mask = 1ULL << gpio,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    esp_err_t err = gpio_config(&config);
    if (err != ESP_OK) return err;
    err = gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) return err;
    return gpio_isr_handler_add(gpio, edge_handler, input);
}

float frequency_count_hz(const frequency_input_t *input)
{
    uint32_t period = input->period_us;
    if (!period || esp_timer_get_time() - input->last_edge_us > 1000000) return 0.0f;
    return 1000000.0f / period;
}
