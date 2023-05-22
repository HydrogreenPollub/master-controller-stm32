/*
 * horn.c
 *
 *  Created on: May 14, 2021
 *      Author: Alicja Miekina
 *      Biblioteka do sterowania systemami bezpieczenstwa
 */

#include "rs485SW.h"
#include "rs485EF.h"
#include "horn.h"
#include "gpio.h"

void safety_init(void){

	HAL_GPIO_WritePin(GPIOC,Solenoid_Valve_GPIO_Pin, GPIO_PIN_SET); //zawor
	HAL_GPIO_WritePin(GPIOB, Emergency_Relay_GPIO_Pin, GPIO_PIN_SET); //przekaznik
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); //kierownica

}
void safety_step(void){
	if(RS485_RX_VERIFIED_DATA_SW.horn == 1){
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_SET);
	}

	if(RS485_RX_VERIFIED_DATA_SW.horn == 0){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); //kierownica
    }

	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == 0) //przycisk bezpieczenstwa
	{
		RS485_TX_DATA_SW.emergencyButton = 1;
		RS485_TX_DATA_EF.emergencyScenario = 1;
		HAL_GPIO_WritePin(GPIOC, Solenoid_Valve_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Emergency_Relay_GPIO_Pin, GPIO_PIN_RESET);
		RS485_TX_DATA_EF.motorPWM = 0;
		RS485_TX_DATA_SW.motorPWM = 0;
	} else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == 1){
		RS485_TX_DATA_SW.emergencyButton = 0;		RS485_TX_DATA_EF.emergencyScenario = 0;
		HAL_GPIO_WritePin(GPIOC,Solenoid_Valve_GPIO_Pin, GPIO_PIN_SET); //zawor
		HAL_GPIO_WritePin(GPIOB, Emergency_Relay_GPIO_Pin, GPIO_PIN_SET);
	}
	if(  RS485_RX_VERIFIED_DATA_SW.powerSupply == 0 ){
		RS485_TX_DATA_SW.emergencyButton = 1;
		RS485_TX_DATA_EF.emergencyScenario = 1;
		HAL_GPIO_WritePin(GPIOC, Solenoid_Valve_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Emergency_Relay_GPIO_Pin, GPIO_PIN_RESET);
		RS485_TX_DATA_EF.motorPWM = 0;
		RS485_TX_DATA_SW.motorPWM = 0;
	}
	if(   RS485_RX_VERIFIED_DATA_EF.emergency == 1){
		RS485_TX_DATA_SW.emergencyButton = 1;
	//	RS485_TX_DATA_EF.emergencyScenario = 1;
		HAL_GPIO_WritePin(GPIOC, Solenoid_Valve_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Emergency_Relay_GPIO_Pin, GPIO_PIN_RESET);
		RS485_TX_DATA_EF.motorPWM = 0;
		RS485_TX_DATA_SW.motorPWM = 0;
	}
	if(RS485_TX_DATA_SW.h2SensorDigitalPin == 1){

		RS485_TX_DATA_SW.emergencyButton = 1;
		RS485_TX_DATA_EF.emergencyScenario = 1;
		HAL_GPIO_WritePin(GPIOC, Solenoid_Valve_GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, Emergency_Relay_GPIO_Pin, GPIO_PIN_RESET);
		RS485_TX_DATA_EF.motorPWM = 0;
		RS485_TX_DATA_SW.motorPWM = 0;
	}
}
