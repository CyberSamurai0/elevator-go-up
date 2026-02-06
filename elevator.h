/**
 * elevator.h
 *
 * Manages elevator state and operations.
 * Author: Colin Saumure
 *
 */

#ifndef ELEVATOR_GO_UP_ELEVATOR_H
#define ELEVATOR_GO_UP_ELEVATOR_H

/***** Includes *****/

#include <stdint.h>
#include "motor-controller.h"
#include "buttons.h"


/***** Variable Declarations *****/

// Bitfield for enabled floors
extern uint16_t enabled_floors;

// Current floor of the elevator on range [1,16]
extern uint8_t current_floor;

// Bitfield for desired floors
extern uint16_t desired_floors;

// Direction of elevator movement: 1 for up, -1 for down, 0 for idle
extern uint8_t direction;


/***** Function Prototypes *****/

uint8_t isFloorEnabled(uint8_t floor);
void enableFloor(uint8_t floor);
void disableFloor(uint8_t floor);

void addDesiredFloor(uint8_t floor);
void removeDesiredFloor(uint8_t floor);

void initElevatorSystem(uint8_t en_pin, uint8_t step_pin, uint8_t dir_pin, uint16_t floors);

#endif //ELEVATOR_GO_UP_ELEVATOR_H