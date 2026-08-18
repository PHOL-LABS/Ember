#pragma once
#include <stddef.h>
#include "esp_err.h"
esp_err_t protocol_status_json(char *buffer, size_t length);
esp_err_t protocol_command(const char *command);
