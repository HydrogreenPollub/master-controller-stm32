/*
 * laptime.c
 *
 *  Created on: May 14, 2021
 *      Author: Alicja Miekina
 *      Biblioteka do odczytu predkosci i sterowania silnikiem
 */

#include "rs485EF.h"
#include "rs485SW.h"
#include "hydrogreen.h"
#include "measurements.h"
#include "timers.h"
#include "tim.h"
#include "adc.h"
#include "interrupt.h"
#include "math.h"

void laptime(void);
void calcSpeed();
void softstart();

volatile uint8_t laptime_seconds = 0;				//czas okrazenia w sekundach
volatile uint16_t laptime_minutes = 0;				//czas okrazenia w minuatch
volatile uint16_t laptime_miliseconds = 0;			//czas okrazenia w milisekundach
volatile uint16_t laptime_milisecond_real = 0;		//calkowity czas okrazenia w milisekundach do pomiaru predkosci sredniej
uint8_t speedPulses = 0;							//ilosc przerwan od czujnika predkosci
volatile uint16_t timeInterimSpeed = 0;				//chwilowa predkosc
volatile uint16_t timeIncreasePWM = 0;				//czas narastania PWM
float calcInterimSpeed = 0;							//obliczona chwilowa predkosc
float lastSpeed =0;
uint8_t PWMMotor = 0;								//PWM silnika
uint8_t calcAverageSpeed = 0;						//obliczona srednia predkosc
uint8_t shortTime = 0;								//
uint8_t i = 0;
uint8_t a = 0;
uint8_t pushButtonFlag = 0;
uint8_t HornIsPressed = 0;
uint16_t HornCounter = 0;

uint8_t	nrOfCalcSpeed = 0;
uint16_t SumOfInterimSpeed = 0;
uint16_t time;
uint16_t prevTime;
uint16_t timeBetweenPulses;

uint16_t speedPulsesToAverage = 0;

float circumference = 1.52f;  						// obwod kola w metrach
uint8_t pulsesPerRevolution = 6; 					//ilosc przerwan na obrot
int timeMeasurement = 500; 							//mniejszy czas do wysylu
uint8_t counter = 0;								//

uint16_t timeOf=0;
uint16_t tick = 0;

uint8_t z = 1;
void measurements_step(void)
{
		laptime();
		calcSpeed();
		softstart();
}

void laptime(void){


	if ( RS485_RX_VERIFIED_DATA_SW.speedReset == 1)
	{
		pushButtonFlag = 1;
	}
	if(pushButtonFlag == 1 && RS485_RX_VERIFIED_DATA_SW.speedReset == 0 ){
		i++;
		pushButtonFlag = 0;
			if(i == 4){
				i = 1;
			}
		}
	if( i ==1){
			laptime_seconds = 0;
			laptime_minutes = 0;
		    laptime_miliseconds = 0;
		    speedPulsesToAverage = 0;
		    z =1;
		    i=2;
	}
	switch(i)
	{
					case 2:
							laptime_miliseconds++;
							laptime_milisecond_real++;
							 //Sekundy
							  if (laptime_miliseconds >= PERIOD_1S)
							 {
								  laptime_miliseconds = 0;
							      laptime_seconds++;
							 }

							//Minuty
							 if (laptime_seconds >= 60)
							 {
								 laptime_seconds = 0;
							     laptime_minutes++;
							 }


					  		for(uint8_t k = 0; k < 2; k++){
					  			RS485_TX_DATA_SW.laptime_miliseconds.array[k] = laptime_miliseconds;
					  		}
					  		for(uint8_t k = 0; k < 1; k++){

					  			RS485_TX_DATA_SW.laptime_minutes.array[k] = laptime_minutes;
					  		}

					  			RS485_TX_DATA_SW.laptime_seconds = laptime_seconds;
					  		    RS485_TX_DATA_SW.averageSpeed = calcAverageSpeed;
						break;
					case 3:

						for(uint8_t k = 0; k < 2; k++){
								RS485_TX_DATA_SW.laptime_miliseconds.array[k] = laptime_miliseconds;
						}
						for(uint8_t k = 0; k < 1; k++){

								RS485_TX_DATA_SW.laptime_minutes.array[k] = laptime_minutes;
						}

								RS485_TX_DATA_SW.laptime_seconds = laptime_seconds;

								if(z == 1){
									counter = speedPulsesToAverage / pulsesPerRevolution ;
									calcAverageSpeed = 3600 *((counter * circumference) / laptime_milisecond_real);
									z++;
								}
								laptime_milisecond_real = 0;
								      RS485_TX_DATA_SW.averageSpeed = calcAverageSpeed;
						break;

					default:
						laptime_seconds = 0;
						laptime_minutes = 0;
					    laptime_miliseconds = 0;
	}
}

void calcSpeed(void)
{

	//SPEED
	//chwilowa zliczenie impulsow np w ciagu 100*milisekundy
    //srednia w trakcie calego przejazdu z tablicy predkosci chwilowych
	//jak liczyc interim speed v=pi*srednica*(impulsy w jakims czasie)/(impulsy na jeden obrot)

							tick = HAL_GetTick();
							timeOf = 	tick - time;

						 	timeInterimSpeed++;

						 	if (timeBetweenPulses > 0){
							 	calcInterimSpeed = 3600 * ( circumference  /( timeBetweenPulses * pulsesPerRevolution));

							 	 if(RS485_RX_VERIFIED_DATA_SW.horn == 0){
							 		 if(!HornIsPressed)
							 		 {
							 			 if(HornCounter > 0)
							 			 {
							 				HornCounter --;
							 			 }
							 			 if(!HornCounter)
							 			 {
							 				 lastSpeed = calcInterimSpeed;
							 			 }
							 		 }
							 		 HornIsPressed = 0;
							 	 }
							 	 else
							 	 {
							 		 HornIsPressed = 1;
							 		 HornCounter = 550;
							 	 }
							}
						 	if (timeOf >= 500){
								calcInterimSpeed = 0;
								lastSpeed=0;
							}

					 if(timeInterimSpeed >= PERIOD_1MS * timeMeasurement){


						 				RS485_TX_DATA_SW.interimSpeed = lastSpeed;
						 			 }


		/*if(RS485_RX_VERIFIED_DATA_SW.horn == 0){
			a=0;
		}
		switch(a){
				case 0:
					tick = HAL_GetTick();
							timeOf = 	tick - time;

						 	timeInterimSpeed++;

						 	if (timeBetweenPulses > 0){
							 	calcInterimSpeed = 3600 * ( circumference  /( timeBetweenPulses * pulsesPerRevolution));
							}
						 	if (timeOf >= 500){
								calcInterimSpeed = 0;
							}
						break;
					 if(timeInterimSpeed >= PERIOD_1MS * timeMeasurement){
					 	RS485_TX_DATA_SW.interimSpeed = calcInterimSpeed;
				   		speedPulses = 0;

					   		timeInterimSpeed = 0;
				case 1:
				 	RS485_TX_DATA_SW.interimSpeed = calcInterimSpeed;
				 	break;
		}

		 } */

}
//ten pwm w zależności od halfgas (0-50) i fullgas(0-100) tych przyciskow,
//fullgas 30 km/h, halfgas 15 km/h

void softstart(void)
{


//softstart od 0 do max/4 PWM w 5 sekund na ten moment 5000ms/25%PWM= 200 ms to +1 PWM
	if(RS485_RX_VERIFIED_DATA_SW.fullGas == 1 && RS485_RX_VERIFIED_DATA_SW.halfGas == 0 ){
		if(calcInterimSpeed >= 0 && calcInterimSpeed < 1){
		timeIncreasePWM++;
		 if(timeIncreasePWM >= PERIOD_1MS * 30 && PWMMotor < 100){
		    	PWMMotor++;
		   		timeIncreasePWM = 0;

		    		RS485_TX_DATA_EF.motorPWM = PWMMotor;
		    		RS485_TX_DATA_SW.motorPWM = PWMMotor;
		    		if(PWMMotor == 100){
		    		calcInterimSpeed = 1;
		    		RS485_TX_DATA_EF.motorPWM = 100;
		    		RS485_TX_DATA_SW.motorPWM = 100;
		    		//PWMMotor = 0;
		    	}
		    }
	}
		if (RS485_RX_VERIFIED_DATA_SW.fullGas == 0 && RS485_RX_VERIFIED_DATA_SW.halfGas == 0 ){
			RS485_TX_DATA_EF.motorPWM = 0;
			RS485_TX_DATA_SW.motorPWM = 0;
			PWMMotor = 0;
		}
	}
	if (RS485_RX_VERIFIED_DATA_SW.fullGas == 0 && RS485_RX_VERIFIED_DATA_SW.halfGas == 0 ){
		RS485_TX_DATA_EF.motorPWM = 0;
		RS485_TX_DATA_SW.motorPWM = 0;
		PWMMotor = 0;
	}
	if (RS485_RX_VERIFIED_DATA_SW.fullGas == 0 && RS485_RX_VERIFIED_DATA_SW.halfGas == 1 ){
		RS485_TX_DATA_EF.motorPWM = 0;
		RS485_TX_DATA_SW.motorPWM = 0;
		PWMMotor = 0;
	}

}
