/** Author: sdaware@mac.com
  ******************************************************************************
  * File Name          : HT_Proc.c
  * Date               : 31/01/2017 
  * Description        : Протокол обмена с PC.
  ******************************************************************************
  *            Интерфейс:
  *              bool HT_IsvcpReady(void)       - ответ получен
  *              void HT_DoCmd(uint8_t type)    - обработка команд 
  *                                               принятых через HyperTerminal 
  *
  *            Поддерживаемые команды:
  *              CLEAR    - очистка спектра
  *              START    - старт набора спектра 
  *              STOP     - останов набора спектра
  *              SP       - запрос спектра
  *              HV       - установка высокого 
  *              DADC     - временная задержка перед стартом АЦП (кол-во 0.1 мкс) 
  *              DSAH     - длительность сигнала сброса УВХ (кол-во 0.1 мкс)
  *              DAC      - вкл./выкл. разравнивания (не 0/0)
  *              ID       - изменение значения USB SERIALNUMBER STRING
  *              JSP      - запрос спектра + параметры набора 
  *              PARAMS?  - считывание данных хранящихся во флеш памяти 
  *              PARAMS   - запись текущих значений ID, HV, SpLen во флеш 
  ******************************************************************************
  */

#include "stm32f2xx.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "usbd_cdc_if.h"
#include "HT.h"
//#include "usart.h"
//#include "test.h"

UART_BuffersTypeDef hVCP_Buff;

// UART1
#define RX_BUF_LENGTH 255

// буфер для приема команд из PC
uint8_t RXbuff[RX_BUF_LENGTH];



/* Private variables ---------------------------------------------------------*/
uint8_t fREADY_comportHT = 0; // флаг прихода команды из PC по COM порту
uint8_t fREADY_vcpHT = 0;     // флаг прихода команды из PC по USB порту
static uint8_t  buffCmd[256];
uint8_t  buffCmdVCP[256];
static char  buffOut[256];
 
/* Private function prototypes -----------------------------------------------*/
static void exclchar(char *s, char chr);
void DoCommand(char* buffCmd);
void SendVCP(const char* Buff, uint16_t Len); 

enum eCMD 
{
	eVACUUM0, eAIR0, ePAUSE0, eVACUUM1, eAIR1, ePAUSE1, eGETPARAMS, eGETPARAM0, eGETPARAM1, eGETPARAM2, eGETPARAM3,
	eSETPARAM0, eSETPARAM1, eSETPARAM2, eSETPARAM3, eSETID, eGETID, eSAVEPARAMS, eGETSTATUS
};

char* CmdTable[] = 
{
  "VACUUM0", "AIR0", "PAUSE0", "VACUUM1", "AIR1", "PAUSE1", "GETPARAMS", "GETPARAM0", "GETPARAM1", "GETPARAM2",
	"GETPARAM3", "SETPARAM0", "SETPARAM1", "SETPARAM2", "SETPARAM3", "SETID", "GETID", "SAVEPARAMS", "GETSTATUS",
	NULL
};

uint8_t HT_IscomportReady(void)
{
   return fREADY_comportHT;
}

uint8_t HT_IsvcpReady(void)
{
   return fREADY_vcpHT;
}
  
void HT_DoCmd(void)
{ 
	char *token;
	char* upr;
	// VCP
	// копируем содержимое приемного буфера в буфер команд для разбора
	strcpy((char* )buffCmd, (const char* )hVCP_Buff.pRxBuffPtr);
	// сброс указателя приемного буфера в начало
	hVCP_Buff.RxXferCount = 0;

	// ЕСЛИ СТРОКА ПУСТАЯ УХОДИМ БЕЗ ОБРАБОТКИ
	if(strlen((char const* )buffCmd) > 0)
	{
		// exclude char ' ' from string
		exclchar((char* )buffCmd, ' ');
		// convert to upper case
		upr = (char* )buffCmd;
		while(*upr != NULL)
		{
			*upr = toupper(*upr);
			++upr;
		}

		 // пробуем разбить строку ----------------------------------------------
		 // Establish string and get the first token:
		 token = strtok( (char* )buffCmd, ";" ); // C4996
		 // Note: strtok is deprecated; consider using strtok_s instead
		 while( token != NULL )
		 {
				// tokens in "string" передаем в функцию обработки
				DoCommand(token);
				// Get next token: 
				token = strtok( NULL, ";" ); // C4996
		 }       
 }      
	// VCP
		fREADY_vcpHT = 0;
}

// обработка команд принятых через HyperTerminal 
void DoCommand(char* buffCmd)
{
  int i = 0;
	float val = 0;
  char str[25];
  char *token, *value;
  
    strcpy(str, buffCmd);
    token = strpbrk( str, "=" ); 
    if(token != 0)
    {
        *token = '\0';
        // значение параметра 
        value = token + 1; 
        if(value != NULL)
        {
           char v[10];
                 
           while(isdigit(value[i]) || (value[i] == '-') || (value[i] == '.'))
            {
              v[i] = value[i];
              i++; 
            }
            v[i] = 0;
           
           val = atof(v);
        }
      }
      i = 0;
      
      // разбор команды
      while(CmdTable[i] != NULL)
      {
        if(strcmp((const char* )str, CmdTable[i]) == 0)
        {
          switch(i)
          {
//					case eVACUUM0:
//						vac1.pump_state = Open;
//						vac1.work_flag = 1;
//						pumpStart(&vac1, &tle);
//						break;
//					case eAIR0:
//						vac1.pump_state = Close;
//						vac1.flag_press_state = 4;
//						vac1.work_flag = 0;
//						pumpStop(&vac1, &tle);
//						break;
//					case ePAUSE0:
//						pumpPause(&vac1, &tle);
//						break;
//					case eVACUUM1:
//						vac2.pump_state = Open;
//						vac2.work_flag = 1;
//						pumpStart(&vac2, &tle);
//						break;
//					case eAIR1:
//						vac2.pump_state = Close;
//						vac2.flag_press_state = 4;
//						vac2.work_flag = 0;
//						pumpStop(&vac2, &tle);
//						break;
//					case ePAUSE1:
//						pumpPause(&vac2, &tle);
//						break;
//					case eGETPARAM0:
//						sprintf(buffOut, "{ID=%d; Low_threshold_press_in_first_camera=%f;}\n\r", params.ID, params.TH_Low_camera1);
//						SendAns(buffOut, strlen(buffOut)); 
//						break;
//					case eGETPARAM1:
//						sprintf(buffOut, "{ID=%d; High_threshold_press_in_first_camera=%f;}\n\r", params.ID, params.TH_High_camera1);
//						SendAns(buffOut, strlen(buffOut)); 
//						break;
//					case eGETPARAM2:
//						sprintf(buffOut, "{ID=%d; Low_threshold_press_in_second_camera=%f;}\n\r", params.ID, params.TH_Low_camera2);
//						SendAns(buffOut, strlen(buffOut)); 
//						break;
//					case eGETPARAM3:
//						sprintf(buffOut, "{ID=%d; High_threshold_press_in_second_camera=%f;}\n\r", params.ID, params.TH_High_camera2);
//						SendAns(buffOut, strlen(buffOut)); 
//						break;
//					case eGETPARAMS:
//						sprintf(buffOut, "{ID=%d; Low_threshold_press_in_first_camera=%f; High_threshold_press_in_first_camera=%f; Low_threshold_press_in_second_camera=%f; High_threshold_press_in_second_camera=%f;}\n\r", 
//															 params.ID, params.TH_Low_camera1, params.TH_High_camera1, params.TH_Low_camera2, params.TH_High_camera2);
//						SendAns(buffOut, strlen(buffOut)); 
//						break;
//					case eGETID:
//						sprintf(buffOut, "{ID=%d}\n\r", params.ID);
//						SendAns(buffOut, strlen(buffOut)); 
//					case eSETPARAM0:
//						params.TH_Low_camera1 = val;
//						break;
//					case eSETPARAM1:
//						params.TH_High_camera1 = val;
//						break;
//					case eSETPARAM2:
//						params.TH_Low_camera2 = val;
//						break;
//					case eSETPARAM3:
//						params.TH_High_camera2 = val;
//						break;
//					case eSETID:
//						params.ID = val;
//					case eSAVEPARAMS:
//						Storage_SetParams();
//						break;
//					case eGETSTATUS:
//						sprintf(buffOut, "{ID=%d; vacuum_valve_in_%d_camera=%d; vacuum_valve_in_%d_camera=%d; air_valve_in_%d_camera=%d; air_valve_in_%d_camera=%d; main_valve=%d; pump_in_%d_camera=%d; pump_in_%d_camera=%d;}\n\r",
//					params.ID, vac1.camera_number, vac1.flag_vac_valve_close, vac2.camera_number, vac2.flag_vac_valve_close, vac1.camera_number, vac1.flag_air_valve_close, vac2.camera_number, 
//					vac2.flag_air_valve_close, flag_main_valve_close, vac1.camera_number, vac1.flag_pump_start, vac2.camera_number, vac2.flag_pump_start);
//						SendAns(buffOut, strlen(buffOut)); 
//						break;
          }
          // команда найдена и обработана
          break;
        }
        ++i;
      }    
}

static void exclchar(char *s, char chr)
{
   int i, j = 0;
   for ( i = 0; s[i] != '\0'; i++ ) /* 'i' moves through all of original 's' */
   {
      if ( s[i] != chr )
      {
         s[j++] = s[i]; /* 'j' only moves after we write a non-'chr' */
      }
   }
   s[j] = '\0'; /* re-null-terminate */
}

void SendAns(const char* Buff, uint16_t Len)
{ 
	// VCP
	SendVCP(Buff, Len);
}

extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

void SendVCP(const char* Buff, uint16_t Len)
{
	uint8_t counter = 0;
	uint32_t ticks = HAL_GetTick();
  uint8_t* pBuff = (uint8_t* )Buff;
  while(CDC_Transmit_HS(pBuff, Len) == USBD_BUSY)
	{
		if((HAL_GetTick() - ticks) > 2)
			++counter;
		
		if(counter > 3)
		{
			// чистим буффер ?
			//sda Flush a Tx FIFO
			(void)USB_FlushTxFifo(hpcd_USB_OTG_HS.Instance, 0x01U);
			
			USBD_CDC_HandleTypeDef   *hcdc = (USBD_CDC_HandleTypeDef*) hUsbDeviceHS.pClassData;
  
			if(hUsbDeviceHS.pClassData != NULL)
			{
				
				hcdc->TxState = 0;
			}		
		}
		if(counter > 10) return;
	}
  //CDC_Transmit_FS(pBuff, Len);
}
