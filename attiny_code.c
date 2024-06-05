//Will Stone lab8.2
//sections of code sampled from professor McLaughlin's github and lecture slides for ECE231 and JDP
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "SSD1306.h"
#include "my_adc_lib.h"

#define REDLED PB1      // Arduino Uno pin 6

int main(void)          //main function
{
  int tempThreshold = 30;       //temperature threshold for the redLED
  DDRB = 1<<REDLED;             //set output to pin6
  unsigned int digValue;        //digital value pulled from the temperature sensor
  unsigned int digTotal = 0;    //array of values for computation
  double avgValue, tempC, tempF, mV;      //converted temperatures
  OLED_Init();                  //initializes the OLED
  adc_init();                   //initializes the ADC

  while (1)                     //creates an infinite loop
  {
    int i;
    for(i = 0; i<8; i++){
      digValue = get_adc();           //gets the digital value
      digTotal = digTotal + digValue;
      _delay_ms(1000);
    }

    avgValue = digTotal/8.0;
    digTotal = 0;
    double tTemp = avgValue;
    mV = ((tTemp*5000)/1024);
    tempC = (mV-750)/10+25;       //temperature in celsius
    tempF = (tempC*9.0/5.0)+32;     //temperature in fahrenheit
    
    if(tempF>tempThreshold){        //evaluates the threshold for the LED
      PORTB |= (1<<REDLED);       //On when over the threshold
    }else{
      PORTB &= ~(1<<REDLED);          // REDLED off when temperature threshold is not met
    }
      int temp1 = tempF*10;
      OLED_GoToLine(3);                   //sends the temp to the OLED
      OLED_DisplayString("Temperature");
      OLED_GoToLine(5);
      OLED_DisplayNumber(10,tempF, 3);
      OLED_DisplayString(".");            //required to be precise to the tenths digit
      int temp = temp1%10;
      OLED_DisplayNumber(10, temp, 1);
      OLED_DisplayString("F");

      temp1 = tempC*10;
      OLED_GoToLine(7);
      OLED_DisplayNumber(10, tempC, 3);
      OLED_DisplayString(".");
      temp = temp1%10;        //required to be precise to the tenths digit
      OLED_DisplayNumber(10, temp, 1);
      OLED_DisplayString("C");
      _delay_ms(100);

  }

}