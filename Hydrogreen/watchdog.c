/**
* @file watchdog.c
* @brief Biblioteka do obslugi watchdoga
* @author Piotr Durakiewicz
* @date 22.10.2020
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/

#include "watchdog.h"
#include "iwdg.h"

// ******************************************************************************************************************************************************** //

uint8_t watchdog_flt; 				///< Zmienna przechowujaca flage bledu watchdoga

// ******************************************************************************************************************************************************** //

/**
* @fn watchdog_init(void)
* @brief Funkcja sprawdzajaca przyczyne zresetowania systemu i inicjalizujaca watchdoga
*/
void watchdog_init(void)
{
  //Warunek sprawdzajacy czy system zostal zresetowany z powodu zaniku zasilania
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) != RESET)
    {
      watchdog_flt = WATCHDOG_FLT_LPWRRST;
    }

  //Warunek sprawdzajacy czy system zaczal prace po wykorzystaniu resetu przez uklad czuwajacy
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
    {
      watchdog_flt = WATCHDOG_FLT_IWDGRST;
    }

  //Warunek sprawdzajacy czy reset systemu zostal wywolany w programie
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) != RESET)
    {
      watchdog_flt = WATCHDOG_FLT_SFTRST;
    }

  //Warunek sprawdzajacy czy reset systemu nastapil poprzez wlaczenie badz odlaczenie zasilania
  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    {
      watchdog_flt = WATCHDOG_FLT_NONE;
    }

  HAL_IWDG_Refresh(&hiwdg); 			//Przeladowanie IWDG
}

/**
* @fn watchdog_step(void)
* @brief Funkcja przeladowujaca IWDG, powinna zostac wywolana wewnatrz hydrogreen_step()
*/
inline void watchdog_step(void)
{
  HAL_IWDG_Refresh(&hiwdg); 			//Przeladowanie IWDG
}
