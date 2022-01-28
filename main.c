/*
 * main.c
 *
 *  Created on: Jan 28, 2022
 *      Author: PUSAMA
 */
/*
int main()
{

	return 0;
}
*/

#define F_CPU 8000000UL			/* Define CPU clock Freq 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>			/* Include delay header file */
#include <stdio.h>			/* Include std i/o library file */
#include "Font_Header.h"

#define Data_Port	 PORTA		/* Define data port for GLCD */
#define Command_Port	 PORTC		/* Define command port for GLCD */
#define Data_Port_Dir	 DDRA		/* Define data port for GLCD */
#define Command_Port_Dir DDRC		/* Define command port for GLCD */

#define RS		 PC0		/* Define control pins */
#define RW		 PC1
#define EN		 PC2
#define CS1		 PC3
#define CS2		 PC4
#define RST		 PC5

#define TotalPage	 8

void GLCD_Command(char Command)		/* GLCD command function */
{
	Data_Port = Command;		/* Copy command on data pin */
	Command_Port &= ~(1 << RS);	/* Make RS LOW for command register*/
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}

void GLCD_Data(char Data)		/* GLCD data function */
{
	Data_Port = Data;		/* Copy data on data pin */
	Command_Port |=  (1 << RS);	/* Make RS HIGH for data register */
	Command_Port &= ~(1 << RW);	/* Make RW LOW for write operation */
	Command_Port |=  (1 << EN);	/* HIGH-LOW transition on Enable */
	_delay_us(5);
	Command_Port &= ~(1 << EN);
	_delay_us(5);
}

void GLCD_Init()			/* GLCD initialize function */
{
	Data_Port_Dir = 0xFF;
	Command_Port_Dir = 0xFF;
	/* Select both left & right half of display & Keep reset pin high */
	Command_Port |= (1 << CS1) | (1 << CS2) | (1 << RST);
	_delay_ms(20);
	GLCD_Command(0x3E);		/* Display OFF */
	GLCD_Command(0x40);		/* Set Y address (column=0) */
	GLCD_Command(0xB8);		/* Set x address (page=0) */
	GLCD_Command(0xB9);		/* Set x address (page=1) */
	GLCD_Command(0xBD);		/* Set x address (page=5) */
	GLCD_Command(0x7F);

	GLCD_Command(0xC0);		/* Set z address (start line=0) */
	GLCD_Command(0x3F);		/* Display ON */
}

void GLCD_ClearAll()			/* GLCD all display clear function */
{
	int i,j;
	/* Select both left & right half of display */
	Command_Port |= (1 << CS1) | (1 << CS2);
	for(i = 0; i < TotalPage; i++)
	{
		GLCD_Command((0xB8) + i);/* Increment page */
		for(j = 0; j < 64; j++)
		{
			GLCD_Data(0);	/* Write zeros to all 64 column */
		}
	}
	GLCD_Command(0x40);		/* Set Y address (column=0) */
	GLCD_Command(0xB8);		/* Set x address (page=0) */
}

void GLCD_String(char page_no, char *str)/* GLCD string write function */
{
	unsigned int i, column;
	unsigned int Page = ((0xB8) + page_no);
	unsigned int Y_address = 0;
	float Page_inc = 0.5;

	Command_Port |= (1 << CS1);	/* Select Left half of display */
	Command_Port &= ~(1 << CS2);
	/*now i will shift to the cs2 half*/
	Command_Port |= (1 << CS2);	/* Select right half of display */
	Command_Port &= ~(1 << CS1);

	GLCD_Command(Page);
	for(i = 0; str[i] != 0; i++)	/* Print char in string till null */
	{
		if (Y_address > (1024-(((page_no)*128)+FontWidth)))
		break;
		if (str[i]!=32)
		{
			for (column=1; column<=FontWidth; column++)
			{
				if ((Y_address+column)==(128*((int)(Page_inc+0.5))))
				{
					if (column == FontWidth)
					break;
					GLCD_Command(0x40);
					Y_address = Y_address + column;
					Command_Port ^= (1 << CS1);
					Command_Port ^= (1 << CS2);
					GLCD_Command(Page + Page_inc);
					Page_inc = Page_inc + 0.5;
				}
				else
				{

					if (column == FontWidth)
					break;
					GLCD_Command(0x40);
					GLCD_Command(0x7F);

					Y_address = Y_address + column;
					Command_Port ^= (1 << CS2);
					Command_Port ^= (1 << CS1);
					GLCD_Command(Page + Page_inc);
					Page_inc = Page_inc + 0.5;
				}
			}
		}
		if (Y_address>(1024-(((page_no)*128)+FontWidth)))
		break;
		if((font[((str[i]-32)*FontWidth)+4])==0 || str[i]==32)
		{
			for(column=0; column<FontWidth; column++)
			{
				GLCD_Data(font[str[i]-32][column]);
				if((Y_address+1)%64==0)
				{
					Command_Port ^= (1 << CS1);
					Command_Port ^= (1 << CS2);
					GLCD_Command((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
		}
		else
		{
			for(column=0; column<FontWidth; column++)
			{
				GLCD_Data(font[str[i]-32][column]);
				if((Y_address+1)%64==0)
				{
					Command_Port ^= (1 << CS1);
					Command_Port ^= (1 << CS2);
					GLCD_Command((Page+Page_inc));
					Page_inc = Page_inc + 0.5;
				}
				Y_address++;
			}
			GLCD_Data(0);
			Y_address++;
			if((Y_address)%64 == 0)
			{
				Command_Port ^= (1 << CS1);
				Command_Port ^= (1 << CS2);
				GLCD_Command((Page+Page_inc));
				Page_inc = Page_inc + 0.5;
			}
		}
	}
	GLCD_Command(0x40);	/* Set Y address (column=0) */
}

int main(void)
{
	unsigned int a,b,c,high,period;
			char frequency[14],duty_cy[7];
	GLCD_Init();		/* Initialize GLCD */
	GLCD_ClearAll();	/* Clear all GLCD display */
	/*GLCD_String(0,":8Mz ");	/* Print String on 0th page of display */





	/*	GLCD_Init();		/* Initialize GLCD */
		PORTD = 0xFF;			/* Turn ON pull-up resistor */

		while(1)
		{
			TCCR1A = 0;
			TCNT1=0;
			TIFR = (1<<ICF1);  	/* Clear ICF (Input Capture flag) flag */

			TCCR1B = 0x41;  	/* Rising edge, no prescaler */
			while ((TIFR&(1<<ICF1)) == 0);
			a = ICR1;  		/* Take value of capture register */
			TIFR = (1<<ICF1);  	/* Clear ICF flag */

			TCCR1B = 0x01;  	/* Falling edge, no prescaler */
			while ((TIFR&(1<<ICF1)) == 0);
			b = ICR1;  		/* Take value of capture register */
			TIFR = (1<<ICF1);  	/* Clear ICF flag */

			TCCR1B = 0x41;  	/* Rising edge, no prescaler */
			while ((TIFR&(1<<ICF1)) == 0);
			c = ICR1;  		/* Take value of capture register */
			TIFR = (1<<ICF1);  	/* Clear ICF flag */

			TCCR1B = 0;  		/* Stop the timer */

			if(a<b && b<c)  	/* Check for valid condition,
						to avoid timer overflow reading */
			{
				high=b-a;
				period=c-a;

				long freq= F_CPU/period;/* Calculate frequency */

							/* Calculate duty cycle */
	            		float duty_cycle =((float) high /(float)period)*100;
				ltoa(freq,frequency,10);

				itoa((int)duty_cycle,duty_cy,10);

				GLCD_Command(0x40);
				GLCD_String(0,"Freq: ");
				GLCD_String(0,frequency);
				GLCD_String(1," Hz    ");

				GLCD_Command(0x7F);
				GLCD_String(2,"Duty: ");
				GLCD_String(2,duty_cy);
				GLCD_String(2," %      ");

			}

			else
			{
				GLCD_ClearAll();
				GLCD_String("OUT OF RANGE!!");
			}
			_delay_ms(50);
		}
		while(1);

}
/*
void init_Ex2(void)
{
	 Timer clock = I/O clock / 64
	TCCR1B = (1<<CS11)|(1<<CS10);
	 Clear ICF1. Clear pending interrupts
	TIFR1   = 1<<ICF1;
	 Enable Timer 1 Capture Event Interrupt
	TIMSK1  = 1<<ICIE1;
}
*/
