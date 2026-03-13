#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"

#include "elevator.h"

#define VERSION_MAJOR '0'
#define VERSION_MINOR '1'
#define VERSION_PATCH '4'

#define BOOT_DEBUG 1

void print_uint16_binary(uint16_t value) {
    // Iterate from the most significant bit (15) down to the least significant bit (0)
    for (int i = 15; i >= 0; i--) {
        // Use a bitmask to check the value of the i-th bit
        if ((value >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n"); // Print a newline character at the end
}

int main() {
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    #if BOOT_DEBUG
    // Wait for USB serial connection before proceeding
    while (!stdio_usb_connected()) {
        // Check for a connection every tenth of a second
        // Blink the LED every half second
        for (uint8_t i=0; i<10; i++) {
            sleep_ms(100);
            if (stdio_usb_connected()) break;
            if (i == 0) cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            if (i == 5) cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        }
    }
    #else
    // Blink LED for four seconds before proceeding
    for (uint8_t i=0; i<4; i++) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }
    #endif

    // Turn on the LED to indicate we're past the boot stage
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    // Print version info
    printf("===== Elevator System v%c.%c.%c =====\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);


    printf("[INIT] Initializing elevator system\n");

    initElevatorSystem(17, 18, 19, 0b11111); // Enable five floors for testing

    printf("[INIT] Initialization completed\n\n");
    while (true) {

        // Print current state for debugging
        if (direction & 0b110) {
            printf("UP -> F %d", target_floor);
        } else if (direction & 0b101) {
            printf("DN -> F %d", target_floor);
        } else if (direction & 0b010) {
            printf("UP at F %d", current_floor);
        } else if (direction & 0b001) {
            printf("DN at F %d", current_floor);
        } else {
            printf("      F %d", current_floor);
        }

        // Print desired floors bitfield
        printf("\t\tDesired: ");
        print_uint16_binary(desired_floors);

        // If we're not moving and we're on the desired floor, clear it
        // Disallows pushing the button for the floor you're currently on
        if (isFloorDesired(current_floor) && direction == 0) removeDesiredFloor(current_floor);

        if (desired_floors == 0) {
            // If no more desired floors, idle
            if (direction != 0) {
                printf("All floors served, idling\n");
                direction = 0; // No desired floors, set to idle
            }
        } else if (direction == 0) {
            // If there are desired floors but we're idle, pick a direction

            // Check if there are desired floors above the current floor
            // (1 << (current_floor + 1)) targets the bit above the current floor
            // 1<<3 = 8 = 0b01000
            // mask - 1 = 0b00111
            // ~(mask - 1) = 0b11000, which gives us all floors above the current floor
            if (desired_floors & (~((1 << (current_floor + 1)) - 1))) {
                // Desired floors exist above current floor
                direction = (direction & 0b100) | 0b010; // Set direction to up (010), preserve moving state
                printf("Setting direction to UP\n");
            } else {
                // Desired floors exist only below current floor
                direction = (direction & 0b100) | 0b001; // Set direction to down (001), preserve moving state
                printf("Setting direction to DOWN\n");
            }
        } else {
            // We are moving and we're supposed to be moving!
            
            //      If active (direction set), check if we're at the next desired floor
            //          If at desired floor
            //              Stop motor, update current floor, remove from desired floors
            //              Linger at current floor for a while
            //              Pick next desired floor based on direction
            //              If no more desired floors in current direction, set direction to idle (which will check opposite direction)
            //          If not at desired floor, continue moving in set direction
            
        }

        // Check if any floors are desired
        // If not, idle
        // If so, check if in current direction
            // If so, set new target floor
            // If not, set new direction
        // If no direction, pick one!
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

