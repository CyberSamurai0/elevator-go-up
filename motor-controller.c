/***** Include Header *****/
#include "motor-controller.h"

/***** Static Variables *****/
static volatile uint32_t _steps_remaining = 0;
static uint8_t _pul_pin_irq = 0;
static motor_complete_callback_t _on_complete = NULL;

// Interrupt handler for PWM Wrap
static void pwm_wrap_isr(void) {
    uint slice_num = pwm_gpio_to_slice_num(_pul_pin_irq);
    
    // Clear IRQ flag
    pwm_clear_irq(slice_num);

    // If the interrupt gets called but no more steps
    if (_steps_remaining == 0) return;

    // Decrement remaining steps
    _steps_remaining--;

    if (_steps_remaining == 0) {
        // Return to 0% duty cycle and disable IRQ
        pwm_set_chan_level(slice_num, pwm_gpio_to_channel(_pul_pin_irq), 0);
        pwm_set_irq_enabled(slice_num, false);

        // If a callback is provided, invoke it
        if (_on_complete) _on_complete();
    }
}

/***** Function Definitions *****/

// Initialize the motor controller GPIO pins and PWM for step control.
void init_motor(uint8_t ena_pin, uint8_t dir_pin, uint8_t pul_pin) {
    // Initialize the Enable pin
    gpio_init(ena_pin);
    gpio_set_dir(ena_pin, GPIO_OUT); // Configure as output
    gpio_put(ena_pin, 0); // Enable (lock) motor initially

    // Initialize the Direction pin
    gpio_init(dir_pin);
    gpio_set_dir(dir_pin, GPIO_OUT); // Configure as output
    gpio_put(dir_pin, 0); // Set initial direction to forward

    /***** Step Pin *****/
    gpio_set_function(pul_pin, GPIO_FUNC_PWM); // Configure step pin for PWM

    uint slice_num = pwm_gpio_to_slice_num(pul_pin); // Get PWM slice number
    uint channel = pwm_gpio_to_channel(pul_pin); // Get PWM channel

    uint16_t top = 1999; // Set PWM top value for resolution
    pwm_set_wrap(slice_num, top); // Set the PWM wrap value
    pwm_set_chan_level(slice_num, channel, 0); // Start with 0%
    pwm_set_clkdiv(slice_num, 250.0f); // 125 MHz / 250 / 2000 = 250 Hz step rate

    pwm_set_counter(slice_num, top);

    // Bind PWM Wrap interrupt to our static handler function
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_wrap_isr);
    
    pwm_set_enabled(slice_num, true); // Enable the PWM slice

    printf("[INIT] Motor controller initialized\n");
}

void enable_motor(uint8_t ena_pin, uint8_t on) {
    // Enable pin is designed as fail closed, so we need logical inverse
    gpio_put(ena_pin, !on);
}

void set_motor_dir(uint8_t dir_pin, uint8_t clockwise) {
    // This is accurate - CW is high, CCW is low
    gpio_put(dir_pin, clockwise);
}

void spin_motor(uint8_t pul_pin) {
    uint slice_num = pwm_gpio_to_slice_num(pul_pin); // Get PWM slice number
    uint channel = pwm_gpio_to_channel(pul_pin); // Get PWM channel
    pwm_set_counter(slice_num, 1999u);

    pwm_set_chan_level(slice_num, channel, 1000u);
}

void stop_spin_motor(uint8_t pul_pin) {
    uint slice_num = pwm_gpio_to_slice_num(pul_pin); // Get PWM slice number
    uint channel = pwm_gpio_to_channel(pul_pin); // Get PWM channel

    pwm_set_chan_level(slice_num, channel, 0);
}