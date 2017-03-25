/******************************************************************************/
/*
  This is an Arduino test sketch for "Voltmeter.h" module for
  "Bench Power Supply" project.

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/

  NOTE: - voltage divider R1=100kOhm, R2=8.2kOhm (for best result use R2 < 10kOhm)
        - input pin A0
        - voltage divider wiki page https://en.wikipedia.org/wiki/Voltage_divider

  BSD license, all text above must be included in any redistribution
*/
/******************************************************************************/
#include <LiquidCrystal_I2C.h> //https://github.com/enjoyneering/LiquidCrystal_I2C
#include <Wire.h>
#include "adc.h"
#include "voltmeter.h"

#define LCD_ROWS         4     //qnt. of lcd rows
#define LCD_COLUMNS      20    //qnt. of lcd columns
#define LCD_SUM_SYMBOL   0xF6  //sum. symbol from the LCD ROM
#define LCD_SPACE_SYMBOL 0x20  //space symbol from the LCD ROM

#define MAX_VOLTAGE      15    //max. voltage, v

float    voltage               = 0;
uint8_t  current_icon          = 0;
uint8_t  icon_lowBattery[8]    = {0x0E, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F};
uint8_t  icon_mediumBattery[8] = {0x0E, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F};
uint8_t  icon_fullBattery[8]   = {0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  setupADC();

  /* LCD connection check */ 
  while (lcd.begin(LCD_COLUMNS, LCD_ROWS) != true)                       //20x4 display
  {
    for (uint8_t i = 0; i > 3; i++)                                      //3 blinks if PCF8574/LCD is not connected or lcd pins declaration is wrong
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
    delay(5000);
  }
  lcd.print("PCF8574 is OK");
  delay(1000);
  lcd.clear();

  /* load custom symbols to CGRAM */
  lcd.createChar(0, icon_lowBattery);
  lcd.createChar(1, icon_mediumBattery);
  lcd.createChar(2, icon_fullBattery);

  /* prints static text */
  lcd.setCursor(0, 1);  
    lcd.print("14bit:");

  lcd.setCursor(14, 1);
    lcd.write(LCD_SUM_SYMBOL);
  
  lcd.setCursor(0, 2);
    lcd.print("10bit:");

  lcd.setCursor(14, 2);
    lcd.write(LCD_SUM_SYMBOL);
}


void loop()
{
  current_icon++;

  /* prints animated battery icon */
  lcd.setCursor(0, 0);
    switch(current_icon)
    {
      case 1:
        lcd.write((uint8_t)0);
        break;
      case 2:
        lcd.write((uint8_t)1);
        break;
      case 3:
        lcd.write((uint8_t)2);
        current_icon = 0;
        break;
    }

  voltage = readOversamplingVoltage(VOLTMETER_PIN, VOLTAGE_DIVIDER);

  /* prints dynamic text */
  lcd.setCursor(6, 1);                                                   //set 7-th colum & 2-nd row. NOTE: first colum & row started at zero
    lcd.print(voltage, 3);
    lcd.print("v ");


  lcd.setCursor(15, 1);
    lcd.print(readOversamplingADC(VOLTMETER_PIN, EXTRA_ADC_RESOLUTION));

  lcd.setCursor(6, 2);
    lcd.print(readVoltage(VOLTMETER_PIN, VOLTAGE_DIVIDER), 3);
    lcd.print("v ");
  
  lcd.setCursor(15, 2);
    lcd.print(readADC(VOLTMETER_PIN));

  /* prints horizontal graph from 0 to MAX_VOLTAGE */
  lcd.printHorizontalGraph('V', 3, voltage, MAX_VOLTAGE);                //name of the bar, row, current value, max. value

  delay(1000);
}
