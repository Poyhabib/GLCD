/*******************************************************************************/
/*******************************************************************************/
/***************************	Author : Mohamed Amer **************************/
/***************************	Date : 12/11/2021	****************************/
/***************************	Layer: MCAL			****************************/
/***************************	Version : 1.00		****************************/
/*******************************************************************************/
/*******************************************************************************/

#include "STD_TYPES.h"
#include "ERR_TYPE.h"
#include "BIT_MATH.h"
#include "TIMER_reg.h"
#include "TIMER_private.h"
#include "TIMER_config.h"
#include "TIMER_interface.h"
#include "DIO_interface.h"
#include "GIE_interface.h"
#include "PORT_interface.h"

static void (*Timer0ovf_pvCallBackFunc)(void) = NULL;


void TIMER_Init(void)
{/*
	Wave generation mode : Normal
	CLR_BIT(TCCR0,TCCR0_WGM00);
	CLR_BIT(TCCR0,TCCR0_WGM01);

	CLK SET PRESCALER = 64
	SET_BIT(TCCR0,TCCR0_CS00);
	SET_BIT(TCCR0,TCCR0_CS01);
	CLR_BIT(TCCR0,TCCR0_CS02);

	OverFlow Interrupt Enable
	SET_BIT(TIMSK,TIMSK_TOIE0);

	TCNT0 = 112;



*/

	/*Wave generation mode : Normal*/
		CLR_BIT(TCCR0,TCCR0_WGM00);
		CLR_BIT(TCCR0,TCCR0_WGM01);

		/*No Oc0*/
		CLR_BIT(TCCR0,TCCR0_COM00);
		CLR_BIT(TCCR0,TCCR0_COM01);

	/*CLK SET PRESCALER = 64*/
		SET_BIT(TCCR0,TCCR0_CS00);
		SET_BIT(TCCR0,TCCR0_CS01);
		CLR_BIT(TCCR0,TCCR0_CS02);

		/*Compare mode enterrupt enable*/
		SET_BIT(TIMSK,TIMSK_OCIE0);


		OCR0=250;





}

uint8 TIMER0ovf_u8SetCallBack(void(*Copy_pvCallBackFunc)(void))
{
	uint8 Local_u8ErrorState=OK;
			if(Copy_pvCallBackFunc!=NULL)
			{
				Timer0ovf_pvCallBackFunc = Copy_pvCallBackFunc;

			}
			else
			{
				Local_u8ErrorState=NULL_POINTER;
			}

			return Local_u8ErrorState;
}

void Timer0_voidSetTimerValue(uint8 Copy_u8Value)
{
	TCNT0 = Copy_u8Value;
}


/*Timer 0 Overflow ISR*/
void __vector_11(void) __attribute__((signal));

void __vector_11(void)
{
	if(Timer0ovf_pvCallBackFunc!=NULL)
	{
		Timer0ovf_pvCallBackFunc();

	}
	else
	{
		/*Do nothing*/
	}

}


/*Timer 0 Compare ISR*/
void __vector_10(void) __attribute__((signal));

void __vector_10(void)
{

	if(Timer0ovf_pvCallBackFunc!=NULL)
		{
			Timer0ovf_pvCallBackFunc();

		}
		else
		{
			/*Do nothing*/
		}



}
