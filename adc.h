/***************************************************************************************************/
/*
  This is an Arduino "adc" module for "Bench Power Supply" project.

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/

  NOTE:    - ADC is optimized for input impedance <= ~10kOhm.
             If a source has higher impedance, the sampling time will depend
             on how long time the source needs to charge the S/H capacitor,
             see p.312 of ATmega328 datasheet. Fairly reliable (not minimal)
             delay is T=5*(R+100kOhm)*14pF, where R - input impedance.
             Example for 10kOhm: 42usec. But at 16MHz, each conversion takes 13 ADC
             clocks or 104usec. So we are OK except the time when input 
             impedance > ~10kOhm and we are switching multiplexer.
           - ADC multiplexer also needs time to switch between analog inputs.
           - For optimum performance & accuracy, the ADC clock should not
             exceed 200 kHz. Arduino default speed is 125 kHz.

  WARNING: - micropocessor will be damaged, if INTERNAL ref. voltage is seted up, 
             and voltage greater than 1.1v is applied to the AFER. pin

  BSD license, all text above must be included in any redistribution
*/
/***************************************************************************************************/
#ifndef adc_h
#define adc_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#define REFERENCE_VOLTAGE            3.265                                           //voltage on AREF pin, set 1.10 or 2.25 to activate "INTERNAL" ref.
                                                                                     //lower VREF provides a higher voltage precision but reduces the
                                                                                     //dynamic range of the input signal 

#define ADC_PRESCALE                 64                                              //adc clock, 16Mhz / 64 = 250kHz
#define DEFAULT_ADC_RESOLUTION       10                                              //default ATmega328 ADC resolution, in bits
#define DEFAULT_ADC_STEPS            pow(2, DEFAULT_ADC_RESOLUTION)                  //2^10bits - 1bit = 1024 steps
#define DEFAULT_ADC_VOLTAGE_STEP     REFERENCE_VOLTAGE / (DEFAULT_ADC_STEPS - 1)     //0.00107v @ 1.10v or 0.00322v @ 3.30v, arduino returns 0..1023

#define EXTRA_ADC_RESOLUTION         4                                               //use 4 bit for the best result & speed, range 1..6bits
#define OVERSAMPLED_ADC_RESOLUTION   DEFAULT_ADC_RESOLUTION + EXTRA_ADC_RESOLUTION   //oversampled resolution
#define OVERSAMPLED_ADC_STEPS        pow(2, OVERSAMPLED_ADC_RESOLUTION)              //2^(10+4)bits - 1bit = 16384 steps
#define OVERSAMPLED_ADC_VOLTAGE_STEP REFERENCE_VOLTAGE / (OVERSAMPLED_ADC_STEPS - 1) //0.000067v @ 1.10v/14bit or 0.000201v @ 3.30v/14bits

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void     setupADC();
void     setPrescaler(uint8_t scale);
uint16_t readADC(uint8_t adc_pin_number);
uint16_t readOversamplingADC(uint8_t adc_pin_number, uint8_t extra_resolution = EXTRA_ADC_RESOLUTION);

#endif
