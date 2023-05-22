/*
 * hydrogensensor.c
 *
 *  Created on: May 14, 2021
 *      Author: Alicja Miekina
 *      Biblioteka do odczytu napiecia czujnika wodoru i baterii
 */

#include "rs485EF.h"
#include "rs485SW.h"
#include "hydrogensensor.h"
#include "gpio.h"
#include "adc.h"
#include "interrupt.h"

#define adcChannel hadc2

uint16_t dataToCalculate[2];
float h2_V = 0;
float baterry_V = 0;

uint16_t timeOfS=0;
uint16_t tickS = 0;
uint16_t timeS;
uint16_t prevTimeS;
uint16_t timeBetweenPulsesS;
float prescaler = 14.54;
uint8_t H2counter = 0;

void hydrogensensor_init(void){

	HAL_ADC_Start_DMA(&adcChannel, (uint32_t*) dataToCalculate, 2);

}
void hydrogensensor_step(void){
static unsigned long int FirstTick;
			//tickS= HAL_GetTick();
			//timeOfS = 	tickS - timeS;
				//if ( timeOfS >= 5000){
					//RS485_TX_DATA_SW.h2SensorDigitalPin = 0;
						//	RS485_TX_DATA_EF.emergencyScenario = 0;
						//}

	h2_V = (((float) dataToCalculate[0])* prescaler)/4095.0f;
	baterry_V = (((float) dataToCalculate[1])* prescaler)/4095.0f;

    RS485_TX_DATA_SW.H2_SENSOR_V.array[0] = h2_V;
    if(HAL_GPIO_ReadPin(H2_Digital_GPIO_Port, H2_Digital_Pin))
    {
    	if(!H2counter)
    	{
    		FirstTick = HAL_GetTick();
    	}
    	H2counter = 1;
    	if(HAL_GetTick() - FirstTick >= 76)
    	{
			RS485_TX_DATA_SW.h2SensorDigitalPin = 0; ///zmiana polaryzacji 22.p5.2023
    	}
    }
    else
    {
    	H2counter = 0;
    	FirstTick = 0;
    	RS485_TX_DATA_SW.h2SensorDigitalPin = 1;
    }

}

