/**
* @file hydrogreen.h
* @brief Plik glowny programu
* @author Piotr Durakiewicz
* @date 22.10.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

#pragma once

#include <stdint-gcc.h>

// ******************************************************************************************************************************************************** //

#define PERIOD_1MS		1
#define PERIOD_1S 		(1000 * PERIOD_1MS)

// ******************************************************************************************************************************************************** //

extern void hydrogreen_main(void);
extern void hydrogreen_hardFault(void);

