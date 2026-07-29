/*
 * Lego Elevator Control System
 * 
 * This code implements the control system for a Lego elevator using a Raspberry Pi Pico W.
 * It manages motor control, floor selection, and direction logic based on desired floors.
 * 
 * Author: Colin Saumure
*/

/***** Includes *****/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"

#include "elevator.h"


/***** Constant Definitions *****/

// Semantic versioning for build tracking
#define VERSION_MAJOR 0
#define VERSION_MINOR 2
#define VERSION_PATCH 1

// Configure motor control pins
#define MOTOR_ENA_PIN 17
#define MOTOR_DIR_PIN 18
#define MOTOR_PUL_PIN 19

// Set to 1 to enable debug mode, which waits for a USB serial connection before proceeding
#define BOOT_DEBUG 1

// Set to 1 to enable motor unit test, which tests motor enable pin and basic spinning functionality
#define MOTOR_UNIT_TEST 0

volatile uint8_t lingering = 0; // Flag to indicate if we're currently lingering at a floor
absolute_time_t linger_end_time; // Timestamp for when the linger period should end
#define LINGER_MS 5000 // Linger time in milliseconds

/***** Function Definitions *****/

// Helper function to print a uint16_t value in binary format
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

void motor_complete_callback(void) {
    if (lingering || direction == 0 || target_floor == 0) {
        return;
    }

    if (direction & 0b010) {
        if (current_floor < 16) current_floor++;
    } else if (direction & 0b001) {
        if (current_floor > 1) current_floor--;
    }

    if (current_floor == target_floor) {
        removeDesiredFloor(current_floor);
        direction &= 0b011; // Clear moving bit, preserve direction
        lingering = 1;
        linger_end_time = delayed_by_ms(get_absolute_time(), LINGER_MS);
        return;
    }

    rotate_motor(MOTOR_PUL_PIN, STEPS_BETWEEN_FLOORS, motor_complete_callback);
}

static void start_next_trip(void) {
    if (desired_floors == 0) {
        direction = 0;
        target_floor = 0;
        return;
    }

    if (direction == 0) {
        direction = chooseDirection();
    }

    target_floor = chooseTargetFloor(direction);
    if (target_floor == current_floor) {
        direction = (direction & 0b010) ? 0b001 : 0b010;
        target_floor = chooseTargetFloor(direction);
    }

    if (target_floor == current_floor) {
        direction = 0;
        target_floor = 0;
        return;
    }

    set_motor_dir(MOTOR_DIR_PIN, (direction & 0b010) ? 1 : 0);
    direction |= 0b100;
    rotate_motor(MOTOR_PUL_PIN, STEPS_BETWEEN_FLOORS, motor_complete_callback);
    printf("[SET] Motor active\n");
}

// Entrypoint function
int main() {
    // Initialize stdio for USB serial communication
    stdio_init_all();

    // Demagnetize the motor during startup
    gpio_init(MOTOR_ENA_PIN);
    gpio_set_dir(MOTOR_ENA_PIN, 1);
    gpio_put(MOTOR_ENA_PIN, 1); // Disable motor initially

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
    printf("===== Elevator System v%d.%d.%d =====\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);

    // Call main initializer function for elevator system
    printf("[INIT] Initializing elevator system\n");
    initElevatorSystem(MOTOR_ENA_PIN, MOTOR_DIR_PIN, MOTOR_PUL_PIN, 0b11111); // Enable five floors for testing

    // If we've made it here, initialization was successful
    printf("[INIT] Initialization completed\n\n");


    #if MOTOR_UNIT_TEST
    // Unit test for motor control functionality
    // Tests enable pin, then direction and spinning functionality
    printf("[TEST] Testing motor operations\n\n");

    // Lock and unlock motor 3x
    printf("[TEST] Motor enable pin test:\n");
    for (uint8_t i=0; i<3; i++) {
        enable_motor(17, 0);
        printf("[TEST] Motor disabled\n");
        sleep_ms(1000);

        enable_motor(17, 1);
        printf("[TEST] Motor enabled\n");
        sleep_ms(1000);
    }

    // Spin back and forth 3x
    printf("[TEST] Motor revolution test:\n");
    for (uint8_t i=0; i<3; i++) {
        // Clockwise for one revolution
        set_motor_dir(MOTOR_DIR_PIN, 1);
        spin_motor(MOTOR_PUL_PIN);
        printf("[TEST] Spin Clockwise for 1s\n");
        sleep_ms(1000);

        stop_spin_motor(MOTOR_PUL_PIN);
        printf("[TEST] Stop spin 0.5s\n");
        sleep_ms(500);

        // Counterclockwise for one revolution
        set_motor_dir(MOTOR_DIR_PIN, 0);
        spin_motor(MOTOR_PUL_PIN);
        printf("[TEST] Spin Counterclockwise for 1s\n");
        sleep_ms(1000);

        stop_spin_motor(MOTOR_PUL_PIN);
        printf("[TEST] Stop spin 0.5s\n");
        sleep_ms(500);
    }

    #endif

    while (true) {
        if (lingering) {
            if (to_ms_since_boot(get_absolute_time()) >= to_ms_since_boot(linger_end_time)) {
                lingering = 0;
                printf("Linger complete\n");

                if (desired_floors == 0) {
                    direction = 0;
                    target_floor = 0;
                    printf("No more requested floors\n");
                } else {
                    direction &= 0b011;
                    if (direction == 0) {
                        direction = chooseDirection();
                    }

                    target_floor = chooseTargetFloor(direction);
                    if (target_floor == current_floor) {
                        direction = (direction & 0b010) ? 0b001 : 0b010;
                        target_floor = chooseTargetFloor(direction);
                    }

                    if (target_floor == current_floor) {
                        direction = 0;
                        target_floor = 0;
                        printf("No more floors in current direction\n");
                    } else {
                        printf("Next target floor: %d\n", target_floor);
                        start_next_trip();
                    }
                }
            } else {
                tight_loop_contents();
                continue;
            }
        }

        // Print current state for debugging
        if (direction & 0b110) {
            printf("UP %d -> F %d", current_floor, target_floor);
        } else if (direction & 0b101) {
            printf("DN %d -> F %d", current_floor, target_floor);
        } else if (direction & 0b010) {
            printf("UP   at F %d", current_floor);
        } else if (direction & 0b001) {
            printf("DN   at F %d", current_floor);
        } else {
            printf("        F %d", current_floor);
        }

        // Print desired floors bitfield
        printf("\t\tDesired: ");
        print_uint16_binary(desired_floors);


        // TODO if lingering, continue to next iter until timer expires
        // From there, pick next target floor based on direction and desired floors

        // Check if we're physically moving or idle
        // If idle
        if (direction == 0 && !lingering) {
            // If we're not moving and we're on the desired floor, clear it
            // Disallows pushing the button for the floor you're currently on
            if (isFloorDesired(current_floor) && direction == 0) removeDesiredFloor(current_floor);
                
            // Check if there are any desired floors
            // If so, pick a direction and target floor
            if (desired_floors != 0) {
                direction = chooseDirection();
                printf("[SET] Dir = %s\n", (direction & 0b010) ? "UP" : "DOWN");
                target_floor = chooseTargetFloor(direction);
                printf("[SET] Target = F%d\n", target_floor);
                start_next_trip();
                continue;
            }
            // If not, stay idle and wait for button press
            tight_loop_contents();
        }

        if ((direction & 0b011) != 0 && (direction & 0b100) == 0 && !lingering) {
            start_next_trip();
        }

        // At this point, we're moving and we're supposed to be!

        
        /*
        if (desired_floors == 0) {
            // If no more desired floors, idle
            if (direction & 0b011 != 0) {
                printf("All floors served, idling\n");
                direction = 0; // No desired floors, set to idle
                target_floor = 0; // Clear target floor
            }
        } else if (direction == 0) {
            // If there are desired floors but we're idle, pick a direction

            
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
        */

        sleep_ms(1000);
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
//              Pick next desired floor based on direction - preserve continuity!!
//              If no more desired floors in current direction, set direction to idle (which will check opposite direction)
//          If not at desired floor, continue moving in set direction
//      Desired floors is a bitfield, but there should still be a single target floor at any time
//      Pushing button while moving to target floor will not change target floor until after complete

