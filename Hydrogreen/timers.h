/**
* @file timers.h
* @brief Biblioteka sluzaca do zarzadzania czasem systemu
* @author Piotr Durakiewicz
* @date 30.10.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/
#pragma once

#include <stdint-gcc.h>

// ******************************************************************************************************************************************************** //

extern void timers_init(void);
extern void timers_main(void);
extern void timers_beforeStep1kHz(void);
extern void timers_afterStep1kHz(void);

// ******************************************************************************************************************************************************** //

extern volatile uint8_t timers_tick500Hz; 		///< Flaga ustawiana co okres T = 2ms
extern volatile uint8_t timers_tick1kHz; 		///< Flaga ustawiana co okres T = 1ms, wykorzystywana przy obiegu glownej petli programu
extern volatile uint8_t timers_tick10kHz; 		///< Flaga ustawiana co okres T = 0,1ms
extern volatile uint8_t timers_mainTimeHours; 		///< Czas pracy systemu - liczba godzin
extern volatile uint8_t timers_mainTimeMinutes; 	///< Czas pracy systemu - liczba minut
extern volatile uint8_t timers_mainTimeSeconds; 	///< Czas pracy systemu - liczba sekund
extern volatile uint16_t timers_mainTimeMiliseconds; 	///< Czas pracy systemu - liczba milisekund

extern uint32_t timers_minSysCyclePeriod;		///< Minimalny zanotowany czas trwania petli hydrogreen_step1kHz()
extern uint32_t timers_maxSysCyclePeriod;		///< Maksymalny zanotowany czas trwania petli hydrogreen_step1kHz()
extern uint32_t timers_avgSysCyclePeriod;		///< Sredni czas trwania petli hydrogreen_step1kHz()

extern volatile uint8_t laptime_flag;
