/**
* @file timers.c
* @brief Biblioteka sluzaca do zarzadzania czasem systemu
* @author Piotr Durakiewicz
* @date 30.10.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

#include "timers.h"
#include "tim.h"
#include "hydrogreen.h"
#include "measurements.h"

// ******************************************************************************************************************************************************** //

static volatile uint32_t timers_sysCycle100kHzCnt;		///< Licznik tickow zegara nastepujacych z czestotliwoscia 100kHz
volatile uint8_t timers_tick1kHz; 				///< Flaga ustawiana co okres T = 1ms
volatile uint8_t timers_tick10kHz; 				///< Flaga ustawiana co okres T = 0,1ms
volatile uint8_t timers_mainTimeHours; 				///< Czas pracy systemu - liczba godzin
volatile uint8_t timers_mainTimeMinutes; 			///< Czas pracy systemu - liczba minut
volatile uint8_t timers_mainTimeSeconds; 			///< Czas pracy systemu - liczba sekund
volatile uint16_t timers_mainTimeMiliseconds; 			///< Czas pracy systemu - liczba milisekund
uint32_t timers_minSysCyclePeriod;				///< Minimalny czas trwania petli hydrogreen_step()
uint32_t timers_maxSysCyclePeriod;				///< Maksymalny czas trwania petli hydrogreen_step()
uint32_t timers_avgSysCyclePeriod;				///< Sredni czas trwania petli hydrogreen_step()
volatile uint8_t laptime_flag;
// ******************************************************************************************************************************************************** //

static inline void timers_step(void);
static inline void setSystemOperatingTime(void);
void timers_init(void);
void timers_beforeStep1kHz(void);
void timers_afterStep1kHz(void);

// ******************************************************************************************************************************************************** //

/**
* @fn timers_init(void)
* @brief Funkcja inicjalizujaca timery
*/
void timers_init(void)
{
  HAL_TIM_Base_Start_IT(&htim6);		//Inicjalizuj TIM6 pracujacy z czestotliwoscia 10kHz
  HAL_TIM_Base_Start_IT(&htim7);		//Inicjalizuj TIM7 pracujacy z czestotliwoscia 100kHz
}

/**
* @fn timers_main(void)
* @brief Glowna funkcja odpowiadajaca za interwaly czasowe wykorzystywane w systemie
*/
static inline void timers_step(void)
{
  timers_tick1kHz = 1;

  setSystemOperatingTime();

}

/**
* @fn setSystemOperatingTime(void)
* @brief Funkcja przeliczajaca czas pracy systemu na milisekundy, sekundy, minuty oraz godziny
*/
static inline void setSystemOperatingTime(void)
{
  timers_mainTimeMiliseconds++;

  //Sekundy
  if (timers_mainTimeMiliseconds >= PERIOD_1S)
    {
      timers_mainTimeMiliseconds = 0;
      timers_mainTimeSeconds++;
    }

  //Minuty
  if (timers_mainTimeSeconds >= 60)
    {
      timers_mainTimeSeconds = 0;
      timers_mainTimeMinutes++;
    }

  //Godziny
  if (timers_mainTimeMinutes >= 60)
    {
      timers_mainTimeMinutes = 0;
      timers_mainTimeHours++;
    }
}

/**
* @fn timers_beforeStep1kHz(void)
* @brief Funkcja sluzaca do obliczania czasu trwania petli hydrogreen_step(), wywolac przed hydrogreen_step()
*/
void timers_beforeStep1kHz(void)
{
  timers_sysCycle100kHzCnt = 0;
}

/**
* @fn timers_afterStep1kHz(void)
* @brief Funkcja sluzaca do obliczania czasu trwania petli hydrogreen_step(), wywolac po hydrogreen_step()
*/
void timers_afterStep1kHz(void)
{
  static uint8_t initFlag;
  static uint32_t actSysCyclePeriod;
  static uint32_t avgSysCyclePeriodSum;
  static uint16_t avgCnt;

  //Warunek wykorzystywany przy inicjalizacji systemu (tylko raz)
  if (!initFlag)
    {
      timers_minSysCyclePeriod = 10 * timers_sysCycle100kHzCnt;
      initFlag = 1;
    }

  actSysCyclePeriod = 10 * timers_sysCycle100kHzCnt; //Przeliczenie otrzymanej wartosci na mikrosekundy

  timers_sysCycle100kHzCnt = 0;

  //Oblicz sredni czas trwania cyklu ze 100 probek
  if (avgCnt <= 100)
    {
      avgSysCyclePeriodSum = avgSysCyclePeriodSum + actSysCyclePeriod;
      avgCnt++;
    }
  else
    {
      timers_avgSysCyclePeriod =  avgSysCyclePeriodSum / avgCnt;
      avgSysCyclePeriodSum = 0;
      avgCnt = 0;
    }

  //Najkrotszy czas trwania cyklu
  if (actSysCyclePeriod < timers_minSysCyclePeriod)
    {
      timers_minSysCyclePeriod =  actSysCyclePeriod;
    }

  //Najdluzszy czas trwania cyklu
  if (actSysCyclePeriod > timers_maxSysCyclePeriod)
    {
      timers_maxSysCyclePeriod = actSysCyclePeriod;
    }
}

void HAL_SYSTICK_Callback(void)
{
  timers_step();

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
    {
      timers_tick10kHz = 1;
    }

  if (htim->Instance == TIM7)
    {
      timers_sysCycle100kHzCnt++;
    }
}
