#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "elevator.h"


int main() {
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // Blink LED for four seconds before beginning
    for (uint8_t i=0; i<4; i++) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    printf("Initializing elevator system...\n");

    initElevatorSystem(17, 18, 19, 0b11111); // Enable five floors for testing

    printf("Main Loop!\n");
    while (true) {
        sleep_ms(1000);
        printf(".");
    }
}

// Let's walk through runtime
// Turn on the onboard LED on Pico W units
// Initialize the elevator system with motor pins and five floors
//      Initialize the motor controller
//          Set motor enable, step, and direction pins
//          Set motor to disabled state initially
//          Set initial direction to forward - this does not correspond to elevator.c direction
//          Configure step pin for PWM control
//      Set enabled floors bitfield
//      Set default values for current floor, desired floors, and direction
//      Initialize floor buttons for enabled floors
//          Bind a callback IRQ to each button GPIO
//          Callback adds the floor to desired floors when pressed
// Enter main loop
//      If idle, check desired floors and set direction accordingly
//      If active (direction set), check if we're at the next desired floor
//          If at desired floor
//              Stop motor, update current floor, remove from desired floors
//              Linger at current floor for a while
//              Pick next desired floor based on direction
//              If no more desired floors in current direction, set direction to idle (which will check opposite direction)
//          If not at desired floor, continue moving in set direction
//      Desired floors is a bitfield, but there should still be a single target floor at any time
//      Pushing button while moving to target floor will not change target floor until after complete
