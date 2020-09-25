/*
 * Infant Incubator Analyzer with LM35.c
 *
 * Created: 11/01/2018 12:29:29
 * Author : Eng. Ahmed Talal
 */ 

#include <avr/io.h>			/*Includes io.h header file where all the Input/Output Registers and its Bits are defined for all AVR microcontrollers*/
#define F_CPU 1000000		/*Defines a macro for the delay.h header file. F_CPU is the microcontroller frequency value for the delay.h header file. Default value of F_CPU in delay.h header file is 1000000(1MHz)*/
#include <util/delay.h>		/*Includes delay.h header file which defines two functions, _delay_ms (millisecond delay) and _delay_us (microsecond delay)*/
#include <stdio.h>
#include <stdlib.h>			/*Includes stdlib.h header file which defines different standard library functions.*/
#include "lcd.h"			/*Includes lcd.h where all LCD interface definitions are*/
#define degree_sysmbol 0xdf


void adc_init(void);								/*ADC Function Declarations*/
int read_adc(unsigned char channel);


int main(void)
{
    init_ports();								/* Ports initialization*/
	adc_init();									/*ADC initialization*/
	Lcd8_Init();								/*LCD Initialization*/
	
	char Temperature[10];
	float celsius;
		 
	//Lcd8_Cmd(0x0C);
	//Lcd8_Cmd(0x82);
	//Lcd8_Write_String("Infant Incubator");
	Lcd8_Cmd(0x0C);
	Lcd8_Cmd(0x80);
	Lcd8_Write_String("Incu.AnalyzerProject");
	_delay_ms(500);
	Lcd8_Cmd(0x0C);
	Lcd8_Cmd(0xC0);
	Lcd8_Write_String("Ist.Uni");
	Lcd8_Cmd(0x0C);
	Lcd8_Cmd(0xCA);
	Lcd8_Write_String("AHMEDTALAL");
	_delay_ms(1000);
	
	//Lcd8_Cmd(0x01);			/*Clear screen*/
	//Lcd8_Cmd(0x80);
	//Lcd8_Write_String("Temperature:");
	

/*Start of infinite loop*/
while(1)
{
	Lcd8_Cmd(0xD4);
	Lcd8_Write_String("Temperature");
	celsius = (read_adc(0)*4.88);
	celsius = (celsius/10.00);
	sprintf(Temperature,"%d%cC  ", (int)celsius, degree_sysmbol);               /* convert integer value to ASCII string */
	Lcd8_Cmd(0xE1);
	Lcd8_Write_String(Temperature);                /* send string data for printing */
	_delay_ms(1000);
	memset(Temperature,0,10);
}
return 0;
}
/*End of Program*/

/*ADC Function Definitions*/
void adc_init(void)
{
	DDRA = 0x00;							        /* Make ADC port as input */
	ADCSRA = 0x87;									/* Enable ADC, with freq/128  */
	ADMUX = 0x40;									/* Vref: Avcc, ADC channel: 0 */
}

int read_adc(unsigned char channel)
{
	ADMUX = 0x40 | (channel & 0x07);				/* set input channel to read */
	ADCSRA |= (1<<ADSC);							/* Start ADC conversion */
	while (!(ADCSRA & (1<<ADIF)));					/* Wait until end of conversion by polling ADC interrupt flag */
	ADCSRA |= (1<<ADIF);							/* Clear interrupt flag */
	_delay_ms(1);									/* Wait a little bit */
	return ADCW;									/* Return ADC word */
}
