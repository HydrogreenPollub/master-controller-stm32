/**
* @file b
* @brief
* @author
* @date
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/
#pragma once

#include <stdint-gcc.h>



extern void hydrogensensor_step(void);
extern void hydrogensensor_init(void);
extern uint8_t H2counter;

extern uint16_t timeS;
extern uint16_t prevTimeS;
extern uint16_t timeBetweenPulsesS;
extern uint16_t timeOfS;
