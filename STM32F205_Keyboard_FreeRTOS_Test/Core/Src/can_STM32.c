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

//#define DEBUG_WAR_CONSOLE_ON
//#define DEBUG_ERR_CONSOLE_ON


#include "canfestival.h"
#include "can.h"
#include "Keyboard_dict.h"

void can_irq_handler(void);


/******************************************************************************
The driver send a CAN message passed from the CANopen stack
INPUT	CAN_PORT is not used (only 1 avaiable)
	Message *m pointer to message to send
OUTPUT	1 if  hardware -> CAN frame
******************************************************************************/
unsigned char canSend(CAN_PORT notused, Message *m)
{
	CAN_TxHeaderTypeDef		header;
	uint32_t				TxMailbox;
	
	if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)==0) //Check free MB
		return 0;								// No free MB for sending

	
 	header.StdId				= m->cob_id;	// Set cob id in IDvA (Standard Format)
	header.RTR					= (m->rtr)? CAN_RTR_REMOTE : CAN_RTR_DATA; 		//1:0 Mailbox Control Register, set remote transmission request and data lenght code
	header.IDE					= CAN_ID_STD;
	header.TransmitGlobalTime	= DISABLE;
	header.DLC					= m->len;
	
	if(HAL_CAN_AddTxMessage(&hcan1, &header, m->data, &TxMailbox) != HAL_OK){
		Error_Handler(); /* Transmission request Error */
		return 0;
	}
	return 1;
}


/******************************************************************************
The driver passes a received CAN message to the stack
INPUT	Message *m pointer to received CAN message
OUTPUT	1 if a message received
******************************************************************************/

unsigned char canReceive(CAN_HandleTypeDef *hcan,	Message *m)
{
	CAN_RxHeaderTypeDef		RxMessage;
	uint8_t					RxData[8];
//	static int st =0;


	
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxMessage, RxData) != HAL_OK)
	{
		/* Reception Error */
		Error_Handler();
		return 0;			// Nothing received
	}
	
	m->cob_id	= RxMessage.StdId;
	m->len		= RxMessage.DLC;
	m->rtr		= (RxMessage.RTR==CAN_RTR_REMOTE)? 1:0;
	memcpy(m->data, RxData, m->len);
		
//	st=~st;	
//	HAL_GPIO_WritePin(D2LED_PORT,D2LED_PIN,(GPIO_PinState)st);
	
	return 1;		// message received
}

UNS8 canChangeBaudRate(CAN_PORT port, char* baud)
{
	return 0;
}


/******************************************************************************
******************************* CAN INTERRUPT  *******************************/
void can_irq_handler(void)
{
	volatile unsigned int status;
	static Message m = Message_Initializer;		// contain a CAN message
		
	if (canReceive(&hcan1, &m))							// a message received
		canDispatch(&_Data, &m);         // process it
	
		//canDispatch(&SlaveTest_Data, &m);         // process it
		//canDispatch(&gene_SYNC_Data, &m);         // process it  
}
