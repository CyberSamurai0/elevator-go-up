#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

const uint16_t steps_per_revolution = 200; // Number of steps per revolution for the motor (1.8deg/step)

void init_motor(uint8_t en_pin, uint8_t step_pin, uint8_t dir_pin);