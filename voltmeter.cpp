/******************************************************************************/
/*
  This is an Arduino "Voltmeter" module for "Bench Power Supply" project.

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/

  NOTE: voltage divider wiki https://en.wikipedia.org/wiki/Voltage_divider

  BSD license, all text above must be included in any redistribution
*/
/******************************************************************************/
#include "voltmeter.h"


/****************************************************************************************/
/*
    readVoltage()

    Reads voltage
*/
/****************************************************************************************/
float readVoltage(uint8_t adc_pin_number, float voltage_divider)
{
  float voltage = 0;

  voltage = ((float)readADC(adc_pin_number) + 0.5) * DEFAULT_ADC_VOLTAGE_STEP / voltage_divider; //0.5 step for rounding

  return (voltage - VOLTMETER_ERROR);
}


/****************************************************************************************/
/*
    readOversamplingVoltage()

    Reads oversampling voltage
*/
/****************************************************************************************/
float readOversamplingVoltage(uint8_t adc_pin_number, float voltage_divider)
{
  float voltage = 0;

  voltage = ((float)readOversamplingADC(adc_pin_number, EXTRA_ADC_RESOLUTION) + 0.5) * OVERSAMPLED_ADC_VOLTAGE_STEP / voltage_divider;

  return (voltage - OVERSAMPLED_VOLTMETER_ERROR);
}
