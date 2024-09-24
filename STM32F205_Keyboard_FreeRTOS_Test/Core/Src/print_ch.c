#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "stm32f2xx_hal.h"
#include "print_ch.h"
//#include "usart.h"

#define PRINT_CH_DEBUG

static char txt_error2[] = {"\fEr2:print_ch()\n\r"};

void print_ch(t_ch_out ch, const char *str, ...)
{
	
	char array[400];

	static char retention[400];
	va_list arg_ptr;

//	if(strlen(str)>(sizeof(array)/sizeof(char)-2)){ 
//		USART_Send(&huart1, txt_error2, strlen(txt_error2) );
//		while(1);
//	}


	va_start (arg_ptr, str); /* format string */
	vsprintf (array, str, arg_ptr);
	va_end (arg_ptr);

	strlcat(retention,array,sizeof(retention));	
	
	switch(ch){
		case CH_ERROR:
			//USART_Send(&huart1, retention, strlen(retention) );
			retention[0]=0;	
		break;
			
		
		case CH_TTY1:
		case CH_TTY2:
		case CH_TTY3:
		case CH_TTY4:
		case CH_TTY5:
		case CH_TTY6:			
			//USART_IT_Send(&huart1, retention, strlen(retention) );//Приводит к сбоям нужно дебажить
#ifdef PRINT_CH_DEBUG
  		//USART_Send(&huart1, retention, strlen(retention) );
#endif		
			retention[0]=0;	
		break;
		
		case CH_RETENTION://Сохранение
		break;	
		
		case CH_RS485:
			//RS485OUT(retention);
			retention[0]=0;														//удаление сохраненных данных
		break;
		
		case CH_NULL:
			retention[0]=0;														//удаление сохраненных данных
		break;	

		case CH_VCM:			//USB Virtual Com Port
			DEBUGOUT_TERM(retention);
			retention[0]=0;														//удаление сохраненных данных
		break;

		//Вывод отладочной инфы на терминал
		case CH_DEBUG_TERM:
			DEBUGOUT_TERM(retention);
			retention[0]=0;														//удаление сохраненных данных
		break;

	}	
}

