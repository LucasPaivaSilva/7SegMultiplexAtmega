/*
 * 7SegMultiplexAtmega.c
 * Created: 4/24/2019 2:52:02 PM
 * Author : Lucas
 */ 

#include "def_principais.h"
#include "LCD.h"
#include <avr/io.h>
#include <avr/interrupt.h>
//int cont = 0;
int x;
int y = 0;
unsigned char digits[4];
unsigned char segValue [16] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E};
	
ISR(TIMER1_COMPA_vect) //interrupção do TC1
{
	y++;
	switch (y)
	{
		case 1:
		PORTC = 0b1110001;
		PORTD = segValue[digits[3]- '0'];
		break;

		case 2:
		PORTC = 0b1110010;
		PORTD = segValue[digits[2]- '0'];
		break;
		
		case 3:
		PORTC = 0b1110100;
		PORTD = segValue[digits[1]- '0'];
		break;
		
		case 4:
		PORTC = 0b1111000;
		PORTD = segValue[digits[0]- '0'];
		y = 0;
		break;	
	}
	
}

int main(void)
{
	DDRD  = 0xFF;	//Conigura como saida
	DDRC  = 0xFF;	//Conigura como saida
    
	TCNT1 = 0;
	OCR1A = 77;
	TCCR1B = (1<<CS10) | (1<<CS12) | (1<<WGM12);//TC1 com prescaler de 1024
	TIMSK1 = (1<<OCIE1A); //habilita a interrupção do T1
	sei();
	
    while (1) 
    {
	for(x=0;x<10000;x++)
	{					
		ident_num(x, digits);
		_delay_ms(100);
	}
		
    }
}

