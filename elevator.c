/***** Include Header *****/
#include "elevator.h"

/***** Variable Definitions *****/

uint16_t enabled_floors;
uint8_t current_floor;
uint16_t desired_floors;
uint8_t direction;


/***** Function Prototypes *****/

// Checks if a floor is enabled via bitfield
uint8_t isFloorEnabled(uint8_t floor);

// Enables a floor by setting the corresponding bit
void enableFloor(uint8_t floor);

// Disables a floor by clearing the corresponding bit
void disableFloor(uint8_t floor);

/**
 * Add a floor to the desired floors bitfield.
 * Ignores the request if the floor is not enabled.
 */
void addDesiredFloor(uint8_t floor);

/**
 * Remove a floor from the desired floors bitfield.
 * Intended for use once the floor has been reached.
 */
void removeDesiredFloor(uint8_t floor);
/***** Function Definitions *****/

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
