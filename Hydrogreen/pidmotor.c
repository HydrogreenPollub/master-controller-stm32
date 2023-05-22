/*
 * pidmotor.c
 *
 *  Created on: May 14, 2021
 *      Author: Alicja Miekina
 *      Biblioteka do sterowania PWM silnika
 */

#include "rs485SW.h"
#include "rs485EF.h"
#include "pidmotor.h"
#include "gpio.h"
#include "measurements.h"

PID_struct Motor_Speed_PID;

void pid_init(void){
	Motor_Speed_PID.previousError = 0; 		//Poprzedni błąd dla członu różniczkującego
	Motor_Speed_PID.Kp = 100.0;					//Wzmocnienie członu proporcjonalnego
	Motor_Speed_PID.Ki =100.0 ;					//Wzmocnienie członu całkującego
	Motor_Speed_PID.Kd = 1.0;					//Wz mocnienie członu różniczkującego
	Motor_Speed_PID.error = 0;
	Motor_Speed_PID.measurement = 0;			//pomiar z czujnika czyli wartosc wejsciowa
	Motor_Speed_PID.pError = 0;
	Motor_Speed_PID.iError = 0;
	Motor_Speed_PID.dError = 0;
	Motor_Speed_PID.samplingTime = 0.001;    		//czas prbokowania
	Motor_Speed_PID.iMax = 60;
	Motor_Speed_PID.iMin = 0;
	Motor_Speed_PID.iPart = 0;
	Motor_Speed_PID.dPart = 0;
	Motor_Speed_PID.PIDmotorValue = 0;
	Motor_Speed_PID.PIDmotorValueMax = 235;
	Motor_Speed_PID.PIDmotorValueMin = 0;
	Motor_Speed_PID.setValue = 0;

}

void pid_step(void){

	if (calcInterimSpeed >=1){
		if(RS485_RX_VERIFIED_DATA_SW.fullGas == 1 && RS485_RX_VERIFIED_DATA_SW.halfGas == 1 ){
			Motor_Speed_PID.PIDmotorValue = 130;
		}else if(RS485_RX_VERIFIED_DATA_SW.fullGas == 1 && RS485_RX_VERIFIED_DATA_SW.halfGas == 0){
			Motor_Speed_PID.PIDmotorValue = 235;
		}else if(RS485_RX_VERIFIED_DATA_SW.fullGas == 0 && RS485_RX_VERIFIED_DATA_SW.halfGas == 0 ){
			Motor_Speed_PID.PIDmotorValue = 0;
		}else if(RS485_RX_VERIFIED_DATA_SW.fullGas == 0 && RS485_RX_VERIFIED_DATA_SW.halfGas == 1 ){
			Motor_Speed_PID.PIDmotorValue = 0; //było bylo 0

		}
		RS485_TX_DATA_EF.motorPWM = Motor_Speed_PID.PIDmotorValue;
		RS485_TX_DATA_SW.motorPWM = Motor_Speed_PID.PIDmotorValue;
/*
		//PWM = max dla speed = 30km/h
			if(RS485_RX_VERIFIED_DATA_SW.fullGas == 1 && RS485_RX_VERIFIED_DATA_SW.halfGas == 0 ){
				Motor_Speed_PID.setValue = 40;
			}
		//PWM = max/2 dla speed = 15km/h
			if(RS485_RX_VERIFIED_DATA_SW.fullGas == 1 && RS485_RX_VERIFIED_DATA_SW.halfGas == 1 ){
				Motor_Speed_PID.setValue = 15;
				Motor_Speed_PID.iError = 0;
			}
		//PWM = 0 dla speed = 0 km/h
			if( RS485_RX_VERIFIED_DATA_SW.fullGas == 0 && RS485_RX_VERIFIED_DATA_SW.halfGas == 0 ){
				RS485_TX_DATA_EF.motorPWM = 0;
				Motor_Speed_PID.setValue = 0;
				Motor_Speed_PID.iError = 0;
			}
			if (RS485_RX_VERIFIED_DATA_SW.fullGas == 0 && RS485_RX_VERIFIED_DATA_SW.halfGas == 1 ){
				RS485_TX_DATA_EF.motorPWM = 0;
				RS485_TX_DATA_SW.motorPWM = 0;
				Motor_Speed_PID.setValue = 0;
				Motor_Speed_PID.iError = 0;
			}

		Motor_Speed_PID.measurement = 	RS485_TX_DATA_SW.interimSpeed;

		Motor_Speed_PID.error = Motor_Speed_PID.setValue - RS485_TX_DATA_SW.interimSpeed;		//obliczenie uchybu

		Motor_Speed_PID.pError = (float)(Motor_Speed_PID.Kp * Motor_Speed_PID.error);		//odpowiedź członu proporcjonalnego

		//Motor_Speed_PID.iError = (float)(Motor_Speed_PID.iError + ((Motor_Speed_PID.previousError + Motor_Speed_PID.error)))*0.5f*Motor_Speed_PID.samplingTime;
		Motor_Speed_PID.iError = Motor_Speed_PID.iError + (Motor_Speed_PID.previousError + Motor_Speed_PID.error)*0.5f*Motor_Speed_PID.samplingTime;//odpowiedź członu całkującego
		if(Motor_Speed_PID.iError >= Motor_Speed_PID.iMax){
			Motor_Speed_PID.iError = Motor_Speed_PID.iMax;
		} else if(Motor_Speed_PID.iError <= Motor_Speed_PID.iMin){
			Motor_Speed_PID.iError = Motor_Speed_PID.iMin;
		}
		Motor_Speed_PID.iPart = Motor_Speed_PID.iError * Motor_Speed_PID.Ki;

		Motor_Speed_PID.dError = (float)(( Motor_Speed_PID.error - Motor_Speed_PID.previousError)/Motor_Speed_PID.samplingTime);//odpowiedź członu różniczkującego
		Motor_Speed_PID.dPart = Motor_Speed_PID.dError * Motor_Speed_PID.Kd;

		Motor_Speed_PID.PIDmotorValue = Motor_Speed_PID.pError + Motor_Speed_PID.iPart + Motor_Speed_PID.dPart;
		if(Motor_Speed_PID.PIDmotorValue >= Motor_Speed_PID.PIDmotorValueMax){
			Motor_Speed_PID.PIDmotorValue = Motor_Speed_PID.PIDmotorValueMax;
		} else if (Motor_Speed_PID.PIDmotorValue <= Motor_Speed_PID.PIDmotorValueMin){
			Motor_Speed_PID.PIDmotorValue = Motor_Speed_PID.PIDmotorValueMin;
		}

		RS485_TX_DATA_EF.motorPWM = Motor_Speed_PID.PIDmotorValue;
		RS485_TX_DATA_SW.motorPWM = Motor_Speed_PID.PIDmotorValue;
		Motor_Speed_PID.previousError = Motor_Speed_PID.error;
		*/
	}
}
