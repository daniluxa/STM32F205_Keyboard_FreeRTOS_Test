/** Author: sdaware@mac.com
  ******************************************************************************
  * File Name          : HT.h
  * Date               : 31/01/2017 
  * Description        : �������� ������ � PC.
  ******************************************************************************
  *            ���������:
  *              bool HT_IsvcpReady(void)       - ����� �������
  *              void HT_DoCmd(void)    - ��������� ������ 
  *                                               �������� ����� HyperTerminal 
  *
  *            �������������� �������:
  *              CLEAR    - ������� �������
  *              START    - ����� ������ ������� 
  *              STOP     - ������� ������ �������
  *              SP       - ������ �������
  *              HV       - ��������� �������� 
  *              ID       - ��������� �������� USB SERIALNUMBER STRING
  *              JSP      - ������ ������� + ��������� ������ 
  *              PARAMS?  - ���������� ������ ���������� �� ���� ������ 
  *              PARAMS   - ������ ������� �������� ID, HV, SpLen �� ���� 
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __hyperterminal_H
#define __hyperterminal_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"
#include "usb_device.h"
#include "ParamDataFlash.h" 
// HyperTerminal

// UART ------------------------------------------------------------------------
typedef enum
{
		Hum 	= 0,
		Temp	= 1
}Sensors_param;

typedef struct
{
	uint8_t slave_addr;
}ModBusRead;

typedef struct
{
  uint8_t             *pTxBuffPtr;      /* Pointer to UART Tx transfer Buffer */  
  uint16_t            TxXferSize;       /* UART Tx Transfer size              */  
  uint16_t            TxXferCount;      /* UART Tx Transfer Counter           */  
  uint8_t             *pRxBuffPtr;      /* Pointer to UART Rx transfer Buffer */ 
  uint16_t            RxXferSize;       /* UART Rx Transfer size              */  
  uint16_t            RxXferCount;      /* UART Rx Transfer Counter           */    
}UART_BuffersTypeDef;

extern UART_BuffersTypeDef hUART_Buff;

// HyperTerminal
extern uint8_t fREADY_comportHT;     // ���� ������� ������� �� PC �� COM �����
extern uint8_t buffCmdUART[];

extern volatile uint16_t ID;
extern volatile float press_camera1;
extern volatile float SP_press_camera1;
extern volatile float TH_camera1;
extern volatile float press_camera2;
extern volatile float SP_press_camera2;
extern volatile float TH_camera2;

uint8_t HT_IscomportReady(void);   
// VCP -------------------------------------------------------------------------
extern UART_BuffersTypeDef hVCP_Buff;
// HyperTerminal
extern uint8_t fREADY_vcpHT;     // ���� ������� ������� �� PC �� USB �����
extern uint8_t buffCmdVCP[];
//extern uint8_t chkChangeParam;

uint8_t HT_IsvcpReady(void);   
   
uint8_t HT_IscomportReady(void);   
uint8_t HT_IsvcpReady(void);
void HT_DoCmd(void);
void SendVCP(const char* Buff, uint16_t Len);
void SendAns(const char* Buff, uint16_t Len);

#ifdef __cplusplus
}
#endif
#endif /* __hyperterminal_H */

/************************ (C) COPYRIGHT SDAWARE **************END OF FILE****/
