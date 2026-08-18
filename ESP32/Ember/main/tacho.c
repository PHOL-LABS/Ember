#include "tacho.h"
#include "frequency_count.h"
#define TACHO_GPIO GPIO_NUM_32
static frequency_input_t input;
esp_err_t tacho_init(void) { return frequency_count_init(&input, TACHO_GPIO); }
float tacho_frequency_hz(void) { return frequency_count_hz(&input); }
