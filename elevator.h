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


/***** Constant Definitions *****/
#define STEPS_BETWEEN_FLOORS 200 // Number of motor steps between floors

/***** Variable Declarations *****/

// Bitfield for enabled floors
extern uint16_t enabled_floors;

// Current floor of the elevator on range [1,16]
extern uint8_t current_floor;

// Bitfield for desired floors
extern uint16_t desired_floors;

// Direction of elevator movement
// 110 = Direction is Up & Moving
// 101 = Direction is Down & Moving
// 010 = Direction is Up
// 001 = Direction is Down
// 000 = Idle
extern uint8_t direction;

// Tracks the floor we are actively moving to. Should be 0 if direction is idle.
extern uint8_t target_floor;

/***** Function Prototypes *****/

void initElevatorSystem(uint8_t en_pin, uint8_t step_pin, uint8_t dir_pin, uint16_t floors);

uint8_t isFloorEnabled(uint8_t floor);
void enableFloor(uint8_t floor);
void disableFloor(uint8_t floor);

void addDesiredFloor(uint8_t floor);
void removeDesiredFloor(uint8_t floor);
uint8_t isFloorDesired(uint8_t floor);


void onFloorButtonPressed(uint gpio, uint32_t event_mask);

#endif //ELEVATOR_GO_UP_ELEVATOR_H