/***** Include Header *****/
#include "elevator.h"

/***** Variable Definitions *****/

uint16_t enabled_floors;
uint8_t current_floor;
uint16_t desired_floors;
uint8_t direction;


/***** Function Prototypes *****/
void initElevatorSystem(uint8_t en_pin, uint8_t step_pin, uint8_t dir_pin, uint16_t floors);
uint8_t isFloorEnabled(uint8_t floor);
void enableFloor(uint8_t floor);
void disableFloor(uint8_t floor);
void addDesiredFloor(uint8_t floor);
void removeDesiredFloor(uint8_t floor);
/***** Function Definitions *****/

// Initialize the elevator system with motor pins and enabled floors
void initElevatorSystem(uint8_t en_pin, uint8_t step_pin, uint8_t dir_pin, uint16_t floors) {
    init_motor(en_pin, step_pin, dir_pin); // Initialize motor controller
    enabled_floors = floors; // Set enabled floors
    current_floor = 1; // Start at first floor
    desired_floors = 0; // No desired floors initially
    direction = 0; // Default to idle

    // Iterate over buttons and initialize those that are enabled
    for (uint8_t floor = 1; floor <= 16; floor++) {
        if (isFloorEnabled(floor)) initFloorButton(floor, onFloorButtonPressed);
    }
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
