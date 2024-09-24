/** Author: sdaware@mac.com
  ******************************************************************************
  * File Name          : ParamDataFlash.h
  * Date               : 24/06/2016 
  * Description        : ������� ������ � ���� �������. 
  *                      
  ******************************************************************************
  *            ���������:
  *
  *             void Storage_InitParams(void)
  *             void Storage_SetParams(void)    -������ ���������� �� ����
  *             
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __paramdataflash_H
#define __paramdataflash_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

// Flash Data Storage ----------------------------------------------------------
extern const uint32_t extMem[];

// ���������(�������� �� ����) �������� ����� ������ �����
// ��� �������� ����� ���������� � ������� ��������� � ���.
typedef struct {
// ����� ������ ����� ������
uint32_t        MemBlockSign;  
// ID ����������
//uint8_t         DevID[13];          // "AMPLANM30001"
	
	uint8_t 				_nodeid;

//uint16_t        HV;            			// �������� �������� ����������
//uint16_t        SpLen;         			// ������ ������ ��� �������
//uint16_t        ADC_Delay;     			// �������� ����� ������� ��� (���-�� 0.1 ���)
//uint16_t        SAH_Reset_Duration; // ����������������� ������� ������ ��� (���-�� 0.1 ���)

//int16_t         Base;          // �������� �����
//uint16_t        Gain;          // ��������
//int16_t         ULin;          // ������������
//int16_t         ULinW;         // ������������
//uint16_t        LLD, ULD;      // 
//uint16_t        ROIL, ROIR;    // 
//uint16_t        Mode;          // 
//uint32_t        MaxTime;       // 
//uint32_t        MaxCNT;        // 

//uint32_t        DelayStartDAC; // ��������(����) ������ ������ HV � ������ ������ ���������
//uint32_t        TFullDAC; 		 // �����(����) ������ ���������� �� 0 �� 4096

} PARAMS_DEVICE;

// ���������(�������) �������� ����� ������ �����
extern PARAMS_DEVICE params;
// ���� ������������ ����� ������ ���������� �� ����
extern uint8_t       fParamInit;
// �������� � ��
extern int32_t      nDACdelay;

// ���������� ��������� ������ - ��������� ���������� - ��� �������� �� ����
// ���� ������ ���� �� ����, �� ������������ ������������� ���������� �������
// � ���� ������
void Storage_InitParams(void);
// ������ ���������� �� ����, ���������� ����� ������ InitParamsStorage
void Storage_SetParams(void);
void ExistParams(void);
void NewParams2Flash(void);
void ReadFlash2Params(void);
// -----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif /* __paramdataflash_H */

/************************ (C) COPYRIGHT ��������� **************END OF FILE****/
