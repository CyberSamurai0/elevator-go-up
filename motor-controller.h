/**
 * motor-controller.h
 *
 * Manages motor control for elevator movement.
 * Author: Colin Saumure
 *
 */

#ifndef ELEVATOR_GO_UP_MOTOR_CONTROLLER_H
#define ELEVATOR_GO_UP_MOTOR_CONTROLLER_H

/***** Includes *****/

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"


/***** Constant Definitions *****/
#define MOTOR_STEPS_PER_REVOLUTION 200 // 1.8 degrees per step

typedef void (*motor_complete_callback_t)(void);

/***** Function Prototypes *****/

// Initialize the motor controller GPIO pins and PWM for step control.
void init_motor(uint8_t ena_pin, uint8_t dir_pin, uint8_t pul_pin);

void enable_motor(uint8_t ena_pin, uint8_t on);
void set_motor_dir(uint8_t dir_pin, uint8_t clockwise);

void spin_motor(uint8_t pul_pin);
void stop_spin_motor(uint8_t pul_pin);

#endif //ELEVATOR_GO_UP_MOTOR_CONTROLLER_H