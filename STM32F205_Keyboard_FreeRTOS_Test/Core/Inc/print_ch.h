//#include "print_ch.h"
#ifndef __PRINT_CH_H
#define __PRINT_CH_H

//*********************************************************
//************ Ретаргет для модуля print_ch ***************
//*********************************************************	
	#include <stdio.h>
	//#include "BlackBox.h"	
	
	//Прямой вывод в debug-term канал
	#define DEBUGOUT_TERM(str) printf("%s",str)


	//Прямой вывод в RS485 канал
	//#define RS485OUT(str) USART3_Send(str,strlen(str));	

//*********************************************************


	typedef enum {
		CH_NULL,
		
		CH_ERROR,			//
		
		CH_RETENTION,		//При выводе в этот виртуальный канал происходит сохранение данных
							//при последующем вызове print_ch с реальным каналом все данные выдаются разом туда
		
		CH_RS485,
		
		CH_DEBUG_TERM,		//Вывод отладочной информации на терминал
		CH_VCM,				//USB Virtual Com Port
		
		CH_TTY1,			//Виртуальный терминал вывода 1
		CH_TTY2,			//Виртуальный терминал вывода 2
		CH_TTY3,			//Виртуальный терминал вывода 3
		CH_TTY4,			//Виртуальный терминал вывода 4
		CH_TTY5,			//Виртуальный терминал вывода 5
		CH_TTY6,			//Виртуальный терминал вывода 6		
		
	}t_ch_out; 

	void print_ch(t_ch_out ch, const char* str, ...);
#endif

