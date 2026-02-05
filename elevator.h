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
extern uint8_t desired_floors[16];
extern uint8_t direction; // 1 for up, -1 for down, 0 for idle

#endif //ELEVATOR_GO_UP_ELEVATOR_H