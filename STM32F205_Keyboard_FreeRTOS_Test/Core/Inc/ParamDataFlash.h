/** Author: sdaware@mac.com
  ******************************************************************************
  * File Name          : ParamDataFlash.h
  * Date               : 24/06/2016 
  * Description        : Функции работы с флеш памятью. 
  *                      
  ******************************************************************************
  *            Интерфейс:
  *
  *             void Storage_InitParams(void)
  *             void Storage_SetParams(void)    -запись параметров на флеш
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

// параметры(хранимые во флеш) задающие режим работы платы
// при загрузке платы копируются в текущие параметры в ОЗУ.
typedef struct {
// метка начала блока данных
uint32_t        MemBlockSign;  
// ID устройства
//uint8_t         DevID[13];          // "AMPLANM30001"
	
	uint8_t 				_nodeid;

//uint16_t        HV;            			// значение высокого напряжения
//uint16_t        SpLen;         			// Размер буфера для спектра
//uint16_t        ADC_Delay;     			// задержка перед стартом АЦП (кол-во 0.1 мкс)
//uint16_t        SAH_Reset_Duration; // продолжительность сигнала сброса УВХ (кол-во 0.1 мкс)

//int16_t         Base;          // смещение шкалы
//uint16_t        Gain;          // усиление
//int16_t         ULin;          // нелинейность
//int16_t         ULinW;         // нелинейность
//uint16_t        LLD, ULD;      // 
//uint16_t        ROIL, ROIR;    // 
//uint16_t        Mode;          // 
//uint32_t        MaxTime;       // 
//uint32_t        MaxCNT;        // 

//uint32_t        DelayStartDAC; // задержка(мсек) старта вывода HV в начале работы программы
//uint32_t        TFullDAC; 		 // время(мсек) вывода напряжение от 0 до 4096

} PARAMS_DEVICE;

// параметры(текущие) задающие режим работы платы
extern PARAMS_DEVICE params;
// флаг выставляется после чтения параметров из флеш
extern uint8_t       fParamInit;
// задержка в мс
extern int32_t      nDACdelay;

// назначение структуры данных - параметры устройства - для хранения на флеш
// если данные есть на флеш, то производится инициализация параметров данными
// с флеш памяти
void Storage_InitParams(void);
// запись параметров на флеш, вызывается после вызова InitParamsStorage
void Storage_SetParams(void);
void ExistParams(void);
void NewParams2Flash(void);
void ReadFlash2Params(void);
// -----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif /* __paramdataflash_H */

/************************ (C) COPYRIGHT Амплитуда **************END OF FILE****/
