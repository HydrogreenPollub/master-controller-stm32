/**
* @file watchdog.h
* @brief Biblioteka do obslugi watchdoga
* @author Piotr Durakiewicz
* @date 22.10.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/
#pragma once

#include <stdint-gcc.h>


// ******************************************************************************************************************************************************** //

///< 0 - ID biblioteki watchdoga

///< Kody bledow
#define WATCHDOG_FLT_NONE 	0x00 		///< Brak bledu
#define WATCHDOG_FLT_LPWRRST 	0x01 		///< Reset z powodu zaniku zasilania ukladu
#define WATCHDOG_FLT_IWDGRST 	0x02 		///< Reset przez uklad watchdoga niezaleznego
#define WATCHDOG_FLT_SFTRST 	0x03 		///< Reset wywolany w programie

extern uint8_t watchdog_flt; 			///< Zmienna przechowujaca flage bledu watchdoga

// ******************************************************************************************************************************************************** //

extern void watchdog_init(void); 		///< Funkcja wykorzystywana przy starcie systemu
extern void watchdog_step(void); 		///< Funkcja przeladowujaca IWDG

