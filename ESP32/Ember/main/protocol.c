#include "protocol.h"
#include <stdio.h>
#include <string.h>
#include "adc.h"
#include "esp_system.h"
#include "speedometer.h"
#include "tacho.h"

esp_err_t protocol_status_json(char *buffer, size_t length)
{
    ember_adc_sample_t adc = ember_adc_latest();
    int written = snprintf(buffer, length,
        "{\"product\":\"Ember\",\"rpm_hz\":%.2f,\"speed_hz\":%.2f,"
        "\"rpm_adc\":%u,\"speed_adc\":%u}",
        tacho_frequency_hz(), speedometer_frequency_hz(), adc.rpm_signal, adc.speed_signal);
    return written > 0 && (size_t)written < length ? ESP_OK : ESP_ERR_INVALID_SIZE;
}

esp_err_t protocol_command(const char *command)
{
    if (!command) return ESP_ERR_INVALID_ARG;
    if (strcmp(command, "restart") == 0) { esp_restart(); }
    return strcmp(command, "status") == 0 ? ESP_OK : ESP_ERR_NOT_SUPPORTED;
}
