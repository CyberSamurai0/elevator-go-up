/**
 * buttons.h
 *
 * Manages elevator button GPIOs.
 * Author: Colin Saumure
 *
 */

#ifndef ELEVATOR_GO_UP_BUTTONS_H
#define ELEVATOR_GO_UP_BUTTONS_H

/***** Includes *****/

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdint.h>


/***** Constant Definitions *****/

#define FLOOR_1_BUTTON_GPIO 0
#define FLOOR_2_BUTTON_GPIO 1
#define FLOOR_3_BUTTON_GPIO 2
#define FLOOR_4_BUTTON_GPIO 3
#define FLOOR_5_BUTTON_GPIO 4
#define FLOOR_6_BUTTON_GPIO 5
#define FLOOR_7_BUTTON_GPIO 6
#define FLOOR_8_BUTTON_GPIO 7
#define FLOOR_9_BUTTON_GPIO 8
#define FLOOR_10_BUTTON_GPIO 9
#define FLOOR_11_BUTTON_GPIO 10
#define FLOOR_12_BUTTON_GPIO 11
#define FLOOR_13_BUTTON_GPIO 12
#define FLOOR_14_BUTTON_GPIO 13
#define FLOOR_15_BUTTON_GPIO 14
#define FLOOR_16_BUTTON_GPIO 15


/***** Function Prototypes *****/

// Get the GPIO pin number for a given floor button.
uint8_t getGPIOForFloor(uint8_t floor);

// Get the floor number for a given GPIO pin.
uint8_t getFloorForGPIO(uint8_t gpio);

// Initialize the GPIO pin for a given floor button as input with pull-up resistor.
void initFloorButton(uint8_t floor, gpio_irq_callback_t callback);

#endif //ELEVATOR_GO_UP_BUTTONS_H