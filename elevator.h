/**
 * elevator.h
 *
 * Manages elevator state and operations.
 * Author: Colin Saumure
 *
 */
#include <stdint.h>

#ifndef ELEVATOR_GO_UP_ELEVATOR_H
#define ELEVATOR_GO_UP_ELEVATOR_H

extern uint8_t current_floor;
extern uint16_t desired_floors; // Bitfield for desired floors
extern uint8_t direction; // 1 for up, -1 for down, 0 for idle

#endif //ELEVATOR_GO_UP_ELEVATOR_H