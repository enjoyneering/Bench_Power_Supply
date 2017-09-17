/***************************************************************************************************/
/*
  This is an Arduino "voltmeter" module for "Bench Power Supply" project.

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/

  NOTE: voltage divider wiki https://en.wikipedia.org/wiki/Voltage_divider

  BSD license, all text above must be included in any redistribution
*/
/***************************************************************************************************/
#include "voltmeter.h"


/****************************************************************************************/
/*
    readVoltage()

    Reads voltage
*/
/****************************************************************************************/
float readVoltage(uint8_t adc_pin_number, float voltage_divider)
{
  return (float)readADC(adc_pin_number) * DEFAULT_ADC_VOLTAGE_STEP / voltage_divider;
}


/****************************************************************************************/
/*
    readOversamplingVoltage()

    Reads oversampling voltage
*/
/****************************************************************************************/
float readOversamplingVoltage(uint8_t adc_pin_number, float voltage_divider)
{
  return (float)readOversamplingADC(adc_pin_number, EXTRA_ADC_RESOLUTION) * OVERSAMPLED_ADC_VOLTAGE_STEP / voltage_divider;
}
