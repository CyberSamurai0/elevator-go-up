/**
 *
 */

#ifndef ELEVATOR_GO_UP_BUTTONS_H
#define ELEVATOR_GO_UP_BUTTONS_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdint.h>

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

// Get the GPIO pin number for a given floor button.
uint8_t getGPIOForFloor(uint8_t floor);

// Initialize the GPIO pin for a given floor button as input with pull-up resistor.
void activateFloorButton(uint8_t floor);

// Callback function for button press interrupt
void onButtonPressed(uint gpio, uint32_t event_mask);

#endif //ELEVATOR_GO_UP_BUTTONS_H