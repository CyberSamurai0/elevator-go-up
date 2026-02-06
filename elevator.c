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
