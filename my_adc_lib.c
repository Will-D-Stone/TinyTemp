/***************************************
 * my_adc_lib.c 
 * Implementation of a library for initializing ADC peripheral
 * and obtaining digitized samples using ATmega328P MCU
 * Version Author           Date        Comment
 * 1.0      D. McLaughlin   4/16/24     initial release as a contributed libary
 * **************************************/

#include "my_adc_lib.h"

#include <avr/io.h>

void adc_init(void)
{
    ADMUX = 0b10000010;   //sets 1.1V IRV, sets ADC2 as channel
    ADCSRA = 0b10000011;   //turn on the ADC, keep ADC single conversion mode
                       //and set division factor-8 for 125kHz ADC clock
}
unsigned int get_adc()
{
    ADCSRA |= (1 << ADSC); // Start ADC conversion
    while ((ADCSRA & (1 << ADIF)) == 0);  // Wait till ADC finishes
    return ADCL | (ADCH << 8);
}