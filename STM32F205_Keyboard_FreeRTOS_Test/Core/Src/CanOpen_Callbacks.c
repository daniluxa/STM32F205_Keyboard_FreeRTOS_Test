#include "CanOpen_Callbacks.h"
#include "main.h"
#include "can.h"
#include "tim.h"
#include "gpio.h"
#include "math.h"
#include "data.h"
#include "storage.h"
#include "canfestival.h"
#include "Keyboard_dict.h"
#include "Keyboard.h"
#include "lss.h"

extern LED_Keyboard_t led_keyboard;;


// FUNCTIONS WHICH ARE PART OF CANFESTIVAL and *must* be implemented here.
void _initialisation(CO_Data* d)
{
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == 0)
	{
		stg_SetDEVmap(d);	
	}
	else
	{
		if(stg_IsMagic())
			stg_GetDEVmap(d);
		else
			stg_SetDEVmap(d);	
	}	
}

void _preOperational(CO_Data* d)
{
	print_ch(CH_TTY1, "\n\r", 0); 	
	print_ch(CH_TTY1, "\tEntering in PRE-OPERATIONAL \n\r", 0); 
//  setState(d, Operational);	
	if(!(*d->iam_a_slave))
	{
		masterSendNMTstateChange(d, 0, NMT_Reset_Node);
	}
}

//------------------------------------------------------------------------------
void _operational(CO_Data* d)
{	print_ch(CH_TTY1, "\n\r", 0);
	print_ch(CH_TTY1, "\tEntering in OPERATIONAL \n\r", 0); 
}

void _lss_StoreConfiguration(CO_Data* d, UNS8 *error, UNS8 *spec_error)
{
	MSG_WAR(0x3DFF, "LSS Store Configuration: ",d->lss_transfer.nodeID);
	
	stg_SetNodeID(d->lss_transfer.nodeID);
}

UNS32 LEDS_ON_FUNC_CB(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	if(Leds_ON == 1)
	{
		uint64_t new_led_state = Leds_State[0] | (uint64_t)Leds_State[1] << 32;
		
		led_keyboard.btn[0] = (new_led_state & 0x3FC00) 					>> 10;
		led_keyboard.btn[1] = (new_led_state & 0xFF00000)					>> 20;
		led_keyboard.btn[2] = (new_led_state & 0x3FC0000000) 			>> 30;
		led_keyboard.btn[3] = (new_led_state & 0xFF0000000000) 		>> 40;
		led_keyboard.btn[4] = (new_led_state & 0x3FC000000000000)	>> 50;
		
		Leds_ON = 0;
	}
	
	return OD_SUCCESSFUL;
}

void CANOPEN_CALL_BACK_INIT(void)
{
	initTimer();
	MSG_WAR(0x3F34, "Entering in the main ", 0);
	
	//----------------------------- INITIALISATION --------------------------------
  _Data.heartbeatError = _heartbeatError;
  _Data.initialisation = _initialisation;
  _Data.preOperational = _preOperational;
  _Data.lss_StoreConfiguration = (lss_StoreConfiguration_t)_lss_StoreConfiguration;
  _Data.operational	= _operational;
  _Data.stopped		= _stopped;

	setState(&_Data, Initialisation);	// Init the state
	setNodeId (&_Data, stg_GetNodeID()/*0x03*/);
	
	RegisterSetODentryCallBack(&_Data, 0x2002, 0x00, &LEDS_ON_FUNC_CB);;
}
