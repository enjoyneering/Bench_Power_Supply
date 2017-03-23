/******************************************************************************/
/*
  This is an Arduino "ADC" module for "Bench Power Supply" project.

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/

  NOTE:    - ADC is optimized for output impedance <= ~10 kohm or less.
             If a source has higher impedance, the sampling time will depend
             on how long time the source needs to charge the S/H capacitor,
             see p.312 of ATmega328 datasheet.
           - ADC multiplexer also needs time to switch between analog inputs.
           - For optimum performance & accuracy, the ADC clock should not
             exceed 200 kHz. Arduino default speed is 125 kHz.
          
  WARNING: - micropocessor will be damaged, if INTERNAL ref. voltage is seted up, 
             and voltage greater than 1.1v is applied to the AFER. pin

  BSD license, all text above must be included in any redistribution
*/
/******************************************************************************/
#include "adc.h"


/**************************************************************************/
/*
    setupADC()

    Sets-up the reference voltage type

    NOTE: - micropocessor will be damaged, if INTERNAL ref. voltage is set up, 
            and voltage greater than 1.1v is applied to the AFER. pin
          - DEFAULT(3.3v or 5v), INTERNAL(1.1v or 2.25v), EXTERNAL
*/
/**************************************************************************/
void setupADC()
{
  if (REFERENCE_VOLTAGE == 1.10 || REFERENCE_VOLTAGE == 2.56)
  {
    analogReference(INTERNAL);
  }
  else
  {
    analogReference(EXTERNAL);
  }
  
}

/**************************************************************************/
/*
    readADC()

    Reads ADC value, steps
*/
/**************************************************************************/
uint16_t readADC(uint8_t adc_pin_number)
{
  //delayMicroseconds(2);                           //experimental! time to charge built-in ADC capacitor & to switch ADC multiplexer
  return analogRead(adc_pin_number);
}

/**************************************************************************/
/*
    readOversamplingADC()

    Reads oversampled ADC value

    NOTE: - for best result & speed use "extra_resolution" = 4, bandwidth is ~29Hz 
          - for more info, see p.8 of "AVR121: Enhancing ADC resolution by oversampling"
*/
/**************************************************************************/
uint16_t readOversamplingADC(uint8_t adc_pin_number, uint8_t extra_resolution)
{
  uint32_t average_adc_value = 0;
  uint16_t number_of_samples = 0;

 if (extra_resolution > 6)                           //safety check - "average_adc_value" doesn't roll over "uint32_t" limit
 {
   extra_resolution = 6;
 }

  number_of_samples = 1 << (extra_resolution << 1);  //1<<(n<<1) is faster than 2^2n=4^n, "pow(4, extra_resolution)"

  for (uint16_t i = 1; i < number_of_samples; i++)
  {
    //delayMicroseconds(2);                          //experimental! time to charge built-in ADC capacitor & to switch ADC multiplexer 
    average_adc_value += analogRead(adc_pin_number);
  }

  return average_adc_value >> extra_resolution;      //y=x>>n is faster than y=x/2^n
}