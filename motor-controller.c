#include "motor-controller.h"


// Initialize the motor controller GPIO pins and PWM for step control.
void init_motor(uint8_t en_pin, uint8_t step_pin, uint8_t dir_pin) {
    // Initialize the Enable pin
    gpio_set_dir(en_pin, GPIO_OUT); // Configure as output
    gpio_put(en_pin, 1); // Disable motor initially TODO not sure which state is disabled

    // Initialize the Direction pin
    gpio_set_dir(dir_pin, GPIO_OUT); // Configure as output
    gpio_put(dir_pin, 0); // Set initial direction to forward

    /**
     * Initialize the Step pin for PWM
     */
    gpio_set_dir(step_pin, GPIO_OUT); // Step Pin
    gpio_set_function(step_pin, GPIO_FUNC_PWM); // Step Pin as PWM

    uint slice_num = pwm_gpio_to_slice_num(step_pin); // Get PWM slice number
    uint channel = pwm_gpio_to_channel(step_pin); // Get PWM channel

    uint16_t top = 255; // Set PWM top value for resolution
    pwm_set_wrap(slice_num, top); // Set the PWM wrap value
    pwm_set_chan_level(slice_num, channel, 0); // Start with 0%
    pwm_set_enabled(slice_num, true); // Enable the PWM slice
}