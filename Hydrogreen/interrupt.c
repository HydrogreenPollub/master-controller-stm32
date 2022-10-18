/*
 * laptime.c
 *
 *  Created on: May 14, 2021
 *      Author: Alicja Miekina
 *      Biblioteka do odczytu przerwan
 */


#include "rs485EF.h"
#include "rs485SW.h"
#include "measurements.h"
#include "hydrogensensor.h"
#include "gpio.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	// liczenie predkosci chwilowej i sredniej

		if(GPIO_Pin == GPIO_PIN_3){

			speedPulses++;
			speedPulsesToAverage++;
					prevTime = time;
				 	time = HAL_GetTick();
				 	timeBetweenPulses = time - prevTime;
		}

		// czujnik wodoru pomiar
			/*if( GPIO_Pin == H2_Digital_Pin ){
				H2counter ++;
				if(H2counter >= 90)
				{
					RS485_TX_DATA_SW.h2SensorDigitalPin = 1;
				}*/


			//prevTimeS = timeS;
		 	//timeS = HAL_GetTick();
		 	//timeBetweenPulsesS = timeS - prevTimeS;

		//}
}
