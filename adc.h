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
#ifndef adc_h
#define adc_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#define REFERENCE_VOLTAGE            3.30                                           //voltage on AREF pin, set 1.1 or 2.25 to use "INTERNAL" ref. 

#define DEFAULT_ADC_RESOLUTION       10                                             //default ATmega328 ADC resolution, bit
#define DEFAULT_ADC_STEPS            pow(2, DEFAULT_ADC_RESOLUTION)                 //2^10bits = 1024 steps
#define DEFAULT_ADC_VOLTAGE_STEP     REFERENCE_VOLTAGE / DEFAULT_ADC_STEPS          //0.0012v @ 1.10v/10 bit or 0.0032v @ 3.30v/10 bit

#define EXTRA_ADC_RESOLUTION         4                                              //1..6 bits, use 4 bit for the best result & speed
#define OVERSAMPLED_ADC_RESOLUTION   DEFAULT_ADC_RESOLUTION + EXTRA_ADC_RESOLUTION  //oversampled resolution
#define OVERSAMPLED_ADC_STEPS        pow(2, OVERSAMPLED_ADC_RESOLUTION)             //2^(10+4)bits = 16384 steps, if EXTRA_ADC_RESOLUTION = 4
#define OVERSAMPLED_ADC_VOLTAGE_STEP REFERENCE_VOLTAGE / OVERSAMPLED_ADC_STEPS      //0.000067v @ 1.10v/14 bit or 0.00020v @ 3.30v/14 bit

void     setupADC();
uint16_t readADC(uint8_t adc_pin_number);
uint16_t readOversamplingADC(uint8_t adc_pin_number, uint8_t extra_resolution = EXTRA_ADC_RESOLUTION);

#endif
