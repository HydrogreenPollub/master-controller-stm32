/**
* @file hydrogreen.c
* @brief Plik glowny programu
* @author Alicja Miekina
* @date 15.06.2021
* @todo
* @bug
* @copyright 2021 HYDROGREEN TEAM
*/

// ******************************************************************************************************************************************************** //

#include "hydrogreen.h"
#include "timers.h"
#include "watchdog.h"
#include "rs485SW.h"
#include "rs485EF.h"
#include "rs485DL.h"
#include "gpio.h"
#include "horn.h"
#include "measurements.h"
#include "hydrogensensor.h"
#include "pidmotor.h"

// ******************************************************************************************************************************************************** //

//#define HYDROGREEN_DEBUG

// ******************************************************************************************************************************************************** //

/**
* @fn hydrogreen_init(void)
* @brief Glowna funkcja inicjalizujaca, powinna zostac wywolana wewnatrz hydrogreen_main()
*/
static void hydrogreen_init(void)
{
  watchdog_init();
  timers_init();
  pid_init();
  hydrogensensor_init();

	 rs485_init_SW();

	 rs485_init_EF();
	 safety_init();

	// rs485_init_DL();

}

/**
* @fn hydrogreen_step1kHz(void)
* @brief Glowna funkcja wykonywana co T = 1ms, powinna zostac wywolana wewnatrz hydrogreen_main()
*/
static inline void hydrogreen_step1kHz(void)
{
#ifdef HYDROGREEN_DEBUG
  HAL_GPIO_WritePin(GPIOA, DBG_Pin, GPIO_PIN_RESET);
#endif

  hydrogensensor_step();
  measurements_step();
  pid_step();
  safety_step();


  watchdog_step();
#ifdef HYDROGREEN_DEBUG
  HAL_GPIO_WritePin(GPIOA, DBG_Pin, GPIO_PIN_SET);
#endif
}

/**
* @fn hydrogreen_step(void)
* @brief Glowna funkcja wykonywana co T = 0,1ms, powinna zostac wywolana wewnatrz hydrogreen_main()
*/
static inline void hydrogreen_step10kHz(void)
{



     rs485_step_SW();

	 rs485_step_EF();

	 rs485_step_DL();

}

/**
* @fn hydrogreen_main(void)
* @brief Glowna funkcja programu, powinna zostac wywolana w pliku main.c, pomiedzy  USER CODE BEGIN 2 a USER CODE END 2
*/
void hydrogreen_main(void)
{
  hydrogreen_init();

  while (1)
    {
      //Sprawdz czy wystapil tick timera nastepujacy z f = 1kHz
      if (timers_tick1kHz)
	{
	  timers_beforeStep1kHz();

	  hydrogreen_step1kHz();

	  timers_afterStep1kHz();

	  timers_tick1kHz = 0;
	}

      //Sprawdz czy wystapil tick timera nastepujacy z f = 10kHz
      if (timers_tick10kHz)
	{
	  hydrogreen_step10kHz();
	  timers_tick10kHz = 0;
	}
    }
}

/**
* @fn hydrogreen_hardFault(void)
* @brief Sygnalizacja wystapienia hard fault'a, wywolac w pliku main.c, w funkcji Error_Handler()
*/
void hydrogreen_hardFault(void)
{
 // HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin , GPIO_PIN_SET);

}

