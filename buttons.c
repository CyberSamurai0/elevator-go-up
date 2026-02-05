#include "buttons.h"

/**
 * Get the GPIO pin number for a given floor button.
 */
uint8_t getGPIOForFloor(uint8_t floor) {
    switch (floor) {
        case 1: return FLOOR_1_BUTTON_GPIO;
        case 2: return FLOOR_2_BUTTON_GPIO;
        case 3: return FLOOR_3_BUTTON_GPIO;
        case 4: return FLOOR_4_BUTTON_GPIO;
        case 5: return FLOOR_5_BUTTON_GPIO;
        case 6: return FLOOR_6_BUTTON_GPIO;
        case 7: return FLOOR_7_BUTTON_GPIO;
        case 8: return FLOOR_8_BUTTON_GPIO;
        case 9: return FLOOR_9_BUTTON_GPIO;
        case 10: return FLOOR_10_BUTTON_GPIO;
        case 11: return FLOOR_11_BUTTON_GPIO;
        case 12: return FLOOR_12_BUTTON_GPIO;
        case 13: return FLOOR_13_BUTTON_GPIO;
        case 14: return FLOOR_14_BUTTON_GPIO;
        case 15: return FLOOR_15_BUTTON_GPIO;
        case 16: return FLOOR_16_BUTTON_GPIO;
        default: return -1; // Invalid floor
    }
}

/**
 * Initialize the GPIO pin for a given floor button as input with pull-up resistor.
 */
void activateFloorButton(uint8_t floor) {
    uint8_t gpio = getGPIOForFloor(floor);
    if (gpio != -1) {
        gpio_set_dir(gpio, GPIO_IN);
        gpio_pull_up(gpio);
    }
}
