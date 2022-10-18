/**
* @file b
* @brief
* @author
* @date
* @todo
* @bug
* @copyright 2020 HYDROGREEN TEAM
*/
#pragma once

#include <stdint-gcc.h>


extern void pid_step(void);
extern void pid_init(void);

typedef struct
{
	float previousError; 		//Poprzedni błąd dla członu różniczkującego
	float Kp;					//Wzmocnienie członu proporcjonalnego
	float Ki;					//Wzmocnienie członu całkującego
	float Kd;					//Wzmocnienie członu różniczkującego
	float error;
	float measurement;			//pomiar z czujnika czyli wartosc wejsciowa
	float pError;
	float iError;
	float dError;
	float samplingTime;    		//czas prbokowania
	float iMax;
	float iMin;
	float iPart;
	float dPart;
	uint16_t PIDmotorValue;
	float PIDmotorValueMax;
	float PIDmotorValueMin;
	float setValue;
}PID_struct;
extern PID_struct Motor_Speed_PID;


