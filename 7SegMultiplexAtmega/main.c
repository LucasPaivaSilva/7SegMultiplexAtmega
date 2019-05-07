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
int stop = 0;
int debouncePB4 = 0;
int debouncePB5 = 0;
unsigned char digits[4];
unsigned char segValue [16] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x18, 0x08, 0x03, 0x46, 0x21, 0x06, 0x0E};
	
ISR(TIMER1_COMPA_vect) //interrupção do TC1
{
	y++;
	switch (y)
	{
		case 1:
		PORTC = 0b1111110;
		PORTD = segValue[digits[3]- '0'];
		break;

		case 2:
		PORTC = 0b1111101;
		PORTD = segValue[digits[2]- '0'];
		break;
		
		case 3:
		PORTC = 0b1111011;
		PORTD = segValue[digits[1]- '0'];
		break;
		
		case 4:
		PORTC = 0b110111;
		PORTD = segValue[digits[0]- '0'];
		y = 0;
		break;	
	}
	
}

ISR(PCINT0_vect) //interrupção do TC1
{
	if ((!tst_bit(PINB, PB4))&&(debouncePB4==0))
	{
		x = 0;
		debouncePB4 = 1;
	}
	else if ((!tst_bit(PINB, PB5))&&(debouncePB5==0))
	{
		if (stop == 1)
		{
			stop = 0;
		} 
		else
		{
			stop = 1;
		}
 		debouncePB5 = 1;
	}
}

int main(void)
{
	DDRD  = 0xFF;	//Conigura como saida
	DDRC  = 0xFF;	//Conigura como saida
	DDRB  = 0x00;	//Configura como entrada
	PORTB = 0xFF;	//Habilita pull up's
	
    PCICR = 1<<PCIE0;
    PCMSK0 = (1<<PCINT4) | (1<<PCINT5);
	
	TCNT1 = 0;
	OCR1A = 77;
	TCCR1B = (1<<CS10) | (1<<CS12) | (1<<WGM12);//TC1 com prescaler de 1024
	TIMSK1 = (1<<OCIE1A); //habilita a interrupção do T1
	sei();
	
    while (1) 
    {
	if (stop == 0)
	{
		x++;
	}			
	ident_num(x, digits);
	_delay_ms(100);
	if (debouncePB4 == 1)
	{
		debouncePB4 = 0; 
	}
	if (debouncePB5 == 1)
	{
		debouncePB5 = 0;
	}
	if (x>=10000)
	{
		x=0;
	}
    }
}

