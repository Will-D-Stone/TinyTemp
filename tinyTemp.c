/*tinyTemp is a code for an ATtiny85 microchip to display temperature in F and C on an OLED display.
Temperature is pulled from a tmp36 temperature sensor. The attiny has fewer pins than the 7805 so pinning was tricky.
OLED, LED, tmp36, and the ISP header pins all need to connect to the 6 available pins on the ATtiny85. 
W. Stone 5/10/2024 original code written for Umass Junior Design Project */

#include "tinyOLED.h"
#include "my_adc_lib.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#define ANALOG_PIN PB1
#define LED PB3
#define MYDELAY 1000


// messages to print on OLED
const char Message1[] PROGMEM = "TEMPERATURE:";

int main(void)
{
  DDRB = 1<<LED;
  int tempThreshold = 30;
  unsigned int digValue;        //digital value pulled from the temperature sensor
  double digTotal = 0.0;
  double avgValue, tempC, tempF, mV;      //converted temperatures and temp values
  char bufferF[4];
  char bufferC[4];
  OLED_init(); // initialize the OLED
  OLED_clear();
  adc_init();

  while (1)
  {
    int j; //samples the temperature every second for 8 seconds to keep the display from updating so quickly
    for(j = 0; j<8; j++){
      digValue = get_adc();           //gets the digital value
      digTotal = digTotal + digValue;
      _delay_ms(MYDELAY);
    }

    avgValue = digTotal/8.0;
    digTotal = 0;
    double tTemp = avgValue;
    mV = ((tTemp*6000)/1024);
    tempC = (tTemp-750)/10+25;       //temperature in celsius
    tempC = tempC / 3.3;
    tempF = (tempC*9.0/5.0)+32;     //temperature in fahrenheit

    OLED_clear();
    if(tempF>tempThreshold){        //evaluates the threshold for the LED
      PORTB |= (1<<LED);       //On when over the threshold
    }else{
      PORTB &= ~(1<<LED);          // REDLED off when temperature threshold is not met
    }
    OLED_cursor(10, 0);    // set cursor position
    OLED_printP(Message1); // print message 1
    OLED_cursor(10, 1);    // set cursor position
    itoa(tempF, bufferF, 10); // Convert dig value to character string for fahrenheit values
    itoa(tempC, bufferC, 10); // Convert dig value to character string for celsius values 
    unsigned char i;
    for (i = 0; i < strlen(bufferF); i++)
    {
      OLED_printC(bufferF[i]);
    }
    double temp;
    int tempI = tempF*10;
    temp = tempI%10;
    char bufferTemp[1];
    itoa(temp, bufferTemp, 10);
    OLED_printC('.');
    OLED_printC(bufferTemp[0]);
    OLED_printC('F');
    OLED_cursor(10,2);

    for (i=0; i < strlen(bufferC); i++)
    {
      OLED_printC(bufferC[i]);
    }
    tempI = tempC*10;
    temp = tempI%10;
    itoa(temp,bufferTemp,10);
    OLED_printC('.');
    OLED_printC(bufferTemp[0]);
    OLED_printC('C');
    _delay_ms(MYDELAY);
  }
}