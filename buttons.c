/***** Include Header *****/
#include "buttons.h"


/***** Function Definitions *****/

// Get the GPIO pin number for a given floor button.
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

// Get the floor number for a given GPIO pin.
uint8_t getFloorForGPIO(uint8_t gpio) {
    switch (gpio) {
        case FLOOR_1_BUTTON_GPIO: return 1;
        case FLOOR_2_BUTTON_GPIO: return 2;
        case FLOOR_3_BUTTON_GPIO: return 3;
        case FLOOR_4_BUTTON_GPIO: return 4;
        case FLOOR_5_BUTTON_GPIO: return 5;
        case FLOOR_6_BUTTON_GPIO: return 6;
        case FLOOR_7_BUTTON_GPIO: return 7;
        case FLOOR_8_BUTTON_GPIO: return 8;
        case FLOOR_9_BUTTON_GPIO: return 9;
        case FLOOR_10_BUTTON_GPIO: return 10;
        case FLOOR_11_BUTTON_GPIO: return 11;
        case FLOOR_12_BUTTON_GPIO: return 12;
        case FLOOR_13_BUTTON_GPIO: return 13;
        case FLOOR_14_BUTTON_GPIO: return 14;
        case FLOOR_15_BUTTON_GPIO: return 15;
        case FLOOR_16_BUTTON_GPIO: return 16;
        default: return -1; // Invalid GPIO
    }
}

// Initialize the GPIO pin for a given floor button as input with pull-up resistor.
void initFloorButton(uint8_t floor, gpio_irq_callback_t callback) {
    uint8_t gpio = getGPIOForFloor(floor);
    if (gpio != -1) {
        gpio_init(gpio);
        gpio_set_dir(gpio, GPIO_IN);
        gpio_pull_up(gpio);
        gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_FALL, true, callback);
    }
}