#ifndef ELEVATOR_GO_UP_MOTOR_CONTROLLER_H
#define ELEVATOR_GO_UP_MOTOR_CONTROLLER_H

/***** Includes *****/

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"


/***** Constant Definitions *****/
#define MOTOR_STEPS_PER_REVOLUTION 200 // 1.8 degrees per step


/***** Function Prototypes *****/

// Initialize the motor controller GPIO pins and PWM for step control.
void init_motor(uint8_t en_pin, uint8_t step_pin, uint8_t dir_pin);

#endif //ELEVATOR_GO_UP_MOTOR_CONTROLLER_H