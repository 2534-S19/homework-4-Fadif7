/*
 * homework4.h
 *
 *  Created on:
 *      Author:
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_
typedef enum {ECEX, ECE2, ECE25, ECE253, ECE2534} diffStates;
static diffStates currentState = ECEX;

// This function initializes the board by turning off the Watchdog Timer.
void initBoard();
bool charFSM(char rChar);

// TODO: Define any constants that are local to homework.c using #define


#endif /* HOMEWORK4_H_ */
