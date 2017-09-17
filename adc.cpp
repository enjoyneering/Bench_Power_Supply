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
#include "adc.h"


/**************************************************************************/
/*
    setupADC()

    Sets-up the reference voltage type

    NOTE: - alower VREF provides a higher voltage precision but reduces the
            dynamic range of the input signal
          - micropocessor will be damaged, if INTERNAL ref. voltage is set up, 
            and voltage greater than 1.1v is applied to the AFER. pin
          - DEFAULT(3.3v or 5v), INTERNAL(1.1v or 2.25v), EXTERNAL
          - after the ADC is enabled, first conversion takes 25 ADC cycles
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
    analogReference(EXTERNAL); //AFER. pin input impedance ~32kOhm
  }
  setPrescaler(ADC_PRESCALE);  //set ADC clock to 16Mhz / 64 = 250kHz
}

/**************************************************************************/
/*
    setPrescaler()

    Sets ADC prescaler, changes ADPSx bits in the ADCSRA register

    ADCSRA byte:

    7-bit  6-bit  5-bit  4-bit  3-bit  2-bit  1-bit  0-bit
    ADEN   ADSC   ADFR   ADIF   ADIE   ADPS2  ADPS1  ADPS0

    - ADEN (ADC Enable) bit enables the ADC. By clearing this bit to zero,
      the ADC is turned off. Turning the ADC off while a conversion is in
      progress will terminate this conversion.

    - ADSC (ADC Start Conversion) bit. In Free Running Mode, you must set
      this bit to start the first conversion. The following conversions will
      be started automatically. In Single Conversion Mode, you must set it
      to start each conversion. This bit will be cleared by hardware when a
      normal conversion is completed. Remember that the first conversion after
      the ADC is enabled is an extended conversion. An extended conversion will
      not clear this bit after completion.

   - ADFR (ADC Free Running Select) bit must be set if you want to use the
     Free Running Mode.

   - ADIF (ADC Interrupt Flag) bit. This bit is set when an ADC conversion is
     completed. If the ADIE bit is set and global interrupts are enabled, the
     ADC Conversion Complete interrupt is executed. ADIF is cleared by hardware
     when executing the corresponding interrupt handling vector. Alternatively,
     ADIF is cleared by writing a logical 1 (!) to the flag. This has a nasty
     side effect : if you modify some other bit of ADCSR using the SBI or the
     CBI instruction, ADIF will be cleared if it has become set before the operation.

   - ADIE (ADC Interrupt Enable) bit : When the ADIE bit is set and global interrupts
     are enabled, the ADC interrupt is activated and the ADC interrupt routine is
     called when a conversion is completed. When cleared, the interrupt is disabled.

   - ADPS (ADC Prescaler Select ) bits. These bits determine the division factor between
     the AVR clock frequency and the ADC clock frequency. The following table describe
     the setting of these bits:

     ADPS2  ADPS1  ADPS0  Division Factor
     0      0      0      2
     0      0      1      2
     0      1      0      4
     0      1      1      8
     1      0      0      16
     1      0      1      32
     1      1      0      64
     1      1      1      128

    NOTE: - ADC clock frequency should 50kHz..200kHz to to get maximum
            resolution from 10-bit ADC
          - if MCU speed is 16Mhz & we set up "Division Factor" = 128 than
            ADC clock is 16Mhz / 128 = 125kHz
*/
/**************************************************************************/
void setPrescaler(uint8_t scale)
{
  switch (scale)
  {
    case 128:             //111
      sbi(ADCSRA, ADPS2);
      sbi(ADCSRA, ADPS1);
      sbi(ADCSRA, ADPS0);
      break;
    case 64:              //110 
      sbi(ADCSRA, ADPS2);
      sbi(ADCSRA, ADPS1);
      cbi(ADCSRA, ADPS0);
      break;
    case 32:              //101 
      sbi(ADCSRA, ADPS2);
      cbi(ADCSRA, ADPS1);
      sbi(ADCSRA, ADPS0);
      break;
    case 16:              //100
      sbi(ADCSRA, ADPS2);
      cbi(ADCSRA, ADPS1);
      cbi(ADCSRA, ADPS0);
      break;
    case 8:               //011
      cbi(ADCSRA, ADPS2);
      sbi(ADCSRA, ADPS1);
      sbi(ADCSRA, ADPS0);
      break;
    case 4:               //010
      cbi(ADCSRA, ADPS2);
      sbi(ADCSRA, ADPS1);
      cbi(ADCSRA, ADPS0);
      break;
    case 2:               //001
      cbi(ADCSRA, ADPS2);
      cbi(ADCSRA, ADPS1);
      sbi(ADCSRA, ADPS0);
      break;
    default:              //111
      sbi(ADCSRA, ADPS2);
      sbi(ADCSRA, ADPS1);
      sbi(ADCSRA, ADPS0);
      break;
  }
}

/****************************************************************************************/
/*
    readADC()

    Reads ADC value, in steps
*/
/****************************************************************************************/
uint16_t readADC(uint8_t adc_pin_number)
{
  analogRead(adc_pin_number);        //skip reading to activate multiplexer & charge ADC capacitor, if input impedance > 10kOhm
  return analogRead(adc_pin_number); //"analogRead" returns from 0 to 1023
}

/****************************************************************************************/
/*
    readOversamplingADC()

    Reads oversampled ADC value

    NOTE: - for best result & speed use "extra_resolution" = 4
          - first conversion, after the ADC is enabled, takes 25 ADC cycles
          - other conversion takes 13 ADC clocks, so if clock is 16 MHz & prescale
            is 128 than bandwidth is 125KHz / 13 cycles * 4^*extra_resolution = 37Hz.
          - for more info, see p.8 of "AVR121: Enhancing ADC resolution by oversampling"
*/
/****************************************************************************************/
uint16_t readOversamplingADC(uint8_t adc_pin_number, uint8_t extra_resolution)
{
  uint32_t average_adc_value = 0;
  uint16_t number_of_samples = 0;

  if (extra_resolution > 6)                          //safety check - "average_adc_value" doesn't roll over "uint32_t" limit
  {
    extra_resolution = 6;
  }

  number_of_samples = 1 << (extra_resolution << 1);  //1<<(n<<1) is faster than 2^2n=4^n, "pow(4, extra_resolution) = 256"
  
  analogRead(adc_pin_number);                        //skip reading to activate multiplexer & charge ADC capacitor, if input impedance > 10kOhm

  for (uint16_t i = 1; i < number_of_samples; i++)
  {
    average_adc_value += analogRead(adc_pin_number); //"analogRead" returns from 0 to 1023
  }

  return average_adc_value >> extra_resolution;      //y=x>>n is faster than y=x/2^n
}
