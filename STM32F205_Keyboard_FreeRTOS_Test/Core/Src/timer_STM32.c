/*
This file is part of CanFestival, a library implementing CanOpen Stack.

Copyright (C): Edouard TISSERANT and Francis DUPIN
AT91 Port: Peter CHRISTEN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// Includes for the Canfestival driver
#include "canfestival.h"
#include "system_stm32f2xx.h"
#include "tim.h" 

// Define the timer registers
#define BASE_TC	
#define ID_TC	
#define TimerAlarm      //Регистр сравнения
#define TimerCounter   	//Регистр счета 

#define TIMER_INTERRUPT_LEVEL		1

void timer_can_irq_handler(void);


/************************** Modul variables **********************************/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;

void initTimer(void)
/******************************************************************************
Initializes the timer, turn on the interrupt and put the interrupt time to zero
INPUT	void
OUTPUT	void
******************************************************************************/
{
//  unsigned int dummy;

  // First, enable the clock of the TIMER
 
  // Disable the clock and the interrupts

  // Clear status bit


  // Set the Mode of the Timer Counter (MCK / 128)
 

  // Enable the clock


  // Open Timer interrupt
 
	MX_TIM6_Init();
	HAL_TIM_Base_Start_IT(&htim6);
	


  // Start Timer

}

void setTimer(TIMEVAL value)
/******************************************************************************
Set the timer for the next alarm.
INPUT	value TIMEVAL (unsigned long)
OUTPUT	void
******************************************************************************/
{	
	__HAL_TIM_SET_AUTORELOAD(&htim6, value);// Add the desired time to timer interrupt time
}

TIMEVAL getElapsedTime(void)
/******************************************************************************
Return the elapsed time to tell the stack how much time is spent since last call.
INPUT	void
OUTPUT	value TIMEVAL (unsigned long) the elapsed time
******************************************************************************/
{
  unsigned int timer = __HAL_TIM_GET_COUNTER(&htim6);	// Copy the value of the running timer
  // Calculate the time difference
  return timer > last_time_set ? timer - last_time_set : last_time_set - timer;
}


//*----------------------------------------------------------------------------
//* Function Name       : timer_can_irq_handler
//* Object              : C handler interrupt function by the interrupts
//*                       assembling routine
//* Output Parameters   : calls TimeDispatch
//*----------------------------------------------------------------------------
void timer_can_irq_handler(void)
{
	last_time_set =   __HAL_TIM_GET_AUTORELOAD(&htim6);
	TimeDispatch();	// Call the time handler of the stack to adapt the elapsed time
}

