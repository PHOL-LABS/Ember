#include "speedometer.h"
#include "frequency_count.h"
#define SPEEDOMETER_GPIO GPIO_NUM_33
static frequency_input_t input;
esp_err_t speedometer_init(void) { return frequency_count_init(&input, SPEEDOMETER_GPIO); }
float speedometer_frequency_hz(void) { return frequency_count_hz(&input); }
