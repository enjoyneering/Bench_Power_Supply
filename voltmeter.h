/***************************************************************************************************/
/*
  This is an Arduino "voltmeter" module for "Bench Power Supply" project.

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/

  NOTE: voltage divider wiki https://en.wikipedia.org/wiki/Voltage_divider

  BSD license, all text above must be included in any redistribution
*/
/***************************************************************************************************/
#ifndef voltmeter_h
#define voltmeter_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
 #include "adc.h"

#define R1_DIVIDER                  100.50                                 //R1 voltage divider in kOhm, min input resistance is 30v/0.001a > 30kOhm 
#define R2_DIVIDER                  8.21                                   //R2 voltage divider in kOhm, ADC optimized for input impedance  < 10kOhm
#define VOLTAGE_DIVIDER             R2_DIVIDER / (R1_DIVIDER + R2_DIVIDER) //voltage divider coeff.

#define VOLTMETER_PIN               A0                                     //voltmeter pin
#define VOLTMETER_ERROR             0.021                                  //for future use
#define OVERSAMPLED_VOLTMETER_ERROR 0.001                                  //for future use

float readVoltage(uint8_t adc_pin_number = VOLTMETER_PIN, float voltage_divider = VOLTAGE_DIVIDER);
float readOversamplingVoltage(uint8_t adc_pin_number = VOLTMETER_PIN, float voltage_divider = VOLTAGE_DIVIDER);

#endif
