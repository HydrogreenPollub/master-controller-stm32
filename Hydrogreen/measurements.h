/*
 * laptime.h
 *
 *  Created on: May 14, 2021
 *      Author: Alicja Miekina
 */

#ifndef MEASUREMENTS_H_
#define MEASUREMENTS_H_


#pragma once

#include <stdint-gcc.h>

#endif /* MEASUREMENTS_H_ */



extern void measurements_step(void);
extern void laptime(void);
extern void calcSpeed(void);
extern void softstart(void);

extern uint8_t speedPulses;
extern uint16_t speedPulsesToAverage;
extern float calcInterimSpeed;
extern uint16_t time;
extern uint16_t prevTime;
extern uint16_t timeBetweenPulses;
extern uint16_t timeOf;
