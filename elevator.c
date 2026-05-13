/***** Include Header *****/
#include "elevator.h"

/***** Variable Definitions *****/

uint16_t enabled_floors;
uint8_t current_floor;
uint16_t desired_floors;
uint8_t direction;
uint8_t target_floor;


/***** Function Prototypes *****/
void initElevatorSystem(uint8_t en_pin, uint8_t step_pin, uint8_t dir_pin, uint16_t floors);
uint8_t isFloorEnabled(uint8_t floor);
void enableFloor(uint8_t floor);
void disableFloor(uint8_t floor);
void addDesiredFloor(uint8_t floor);
void removeDesiredFloor(uint8_t floor);
uint8_t isFloorDesired(uint8_t floor);
void onFloorButtonPressed(uint gpio, uint32_t event_mask);
uint8_t chooseDirection();
uint8_t chooseTargetFloor(uint8_t direction);


/***** Function Definitions *****/

// Initialize the elevator system with motor pins and enabled floors
void initElevatorSystem(uint8_t en_pin, uint8_t dir_pin, uint8_t step_pin, uint16_t floors) {
    printf("[INIT] Initializing motor controller\n");
    printf("[INIT] EN: GPIO %d, DIR: GPIO %d, STEP: GPIO %d\n", en_pin, dir_pin, step_pin);
    init_motor(en_pin, dir_pin, step_pin); // Initialize motor controller
    enabled_floors = floors; // Set enabled floors
    current_floor = 1; // Start at first floor
    desired_floors = 0; // No desired floors initially
    direction = 0; // Default to idle

    // Iterate over buttons and initialize those that are enabled
    for (uint8_t floor = 1; floor <= 16; floor++) {
        if (isFloorEnabled(floor)) {
            printf("[INIT] Enabling floor %d\n", floor);
            initFloorButton(floor, onFloorButtonPressed);
        }
    }

    printf("[INIT] Elevator system initialized\n");
}

// Checks if a floor is enabled via bitfield
uint8_t isFloorEnabled(uint8_t floor) {
    return (enabled_floors & (1 << (floor - 1))) != 0;
}

// Enables a floor by setting the corresponding bit
void enableFloor(uint8_t floor) {
    enabled_floors |= (1 << (floor - 1));
    initFloorButton(floor, onFloorButtonPressed);
}

// Disables a floor by clearing the corresponding bit
void disableFloor(uint8_t floor) {
    enabled_floors &= ~(1 << (floor - 1));
}

/**
 * Add a floor to the desired floors bitfield.
 * Ignores the request if the floor is not enabled.
 */
void addDesiredFloor(uint8_t floor) {
    if (!isFloorEnabled(floor)) return; // Ignore if floor is not enabled
    desired_floors |= (1 << (floor - 1));
}

/**
 * Remove a floor from the desired floors bitfield.
 * Intended for use once the floor has been reached.
 */
void removeDesiredFloor(uint8_t floor) {
    desired_floors &= ~(1 << (floor - 1));
}

// Checks if a floor is set in the desired_floors bitfield
uint8_t isFloorDesired(uint8_t floor) {
    return desired_floors & (1 << (floor - 1));
}

// Callback for when a floor button is pressed
void onFloorButtonPressed(uint gpio, uint32_t event_mask) {
    uint8_t floor = getFloorForGPIO(gpio);
    printf("Floor %d Pressed\n", floor);
    if (floor != (uint8_t)(-1)) {
        addDesiredFloor(floor);
    }
}

uint8_t chooseDirection() {
    // Check if there are desired floors above the current floor
    // (1 << (current_floor)) targets the bit above the current floor
    // 1<<3 = 8 = 0b01000
    // mask - 1 = 0b00111
    // ~(mask - 1) = 0b11000, which gives us all floors above the current floor
    uint16_t above_mask = ~((1 << (current_floor)) - 1);
    if (desired_floors & above_mask) {
        return 0b010; // UP
    } else if (desired_floors != 0) {
        return 0b001; // DOWN
    } else return 0b000; // No desired floors, should not be used
}

uint8_t chooseTargetFloor(uint8_t direction) {
    if (direction & 0b010) {
        // If direction is up, find the closest desired floor above current floor
        for (int8_t f = current_floor + 1; f <= 16; f++) {
            if (isFloorDesired(f)) {
                return f;
            }
        }
    } else if (direction & 0b001) {
        // If direction is down, find the closest desired floor below current floor
        for (int8_t f = current_floor - 1; f >= 1; f--) {
            if (isFloorDesired(f)) {
                return f;
            }
        }
    }
    return current_floor; // No desired floors in current direction, stay at current floor (should not happen if called correctly)
}