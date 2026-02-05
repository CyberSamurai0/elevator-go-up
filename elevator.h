/**
 * elevator.h
 *
 * Manages elevator state and operations.
 * Author: Colin Saumure
 *
 */
#include <stdint.h>

uint8_t current_floor;
uint8_t desired_floors[16];
uint8_t direction; // 1 for up, -1 for down, 0 for idle

