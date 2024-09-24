/** Author: sdaware@mac.com
  ******************************************************************************
  * File Name          : ParamDataFlash.c
  * Date               : 22/06/2016 
  * Description        : Функции работы с флеш памятью.
  ******************************************************************************
  *     Flash STM32F205RBT
  *     Sector 0 0x0800 0000 - 0x0800 3FFF 16 Kbyte
  *     Sector 1 0x0800 4000 - 0x0800 7FFF 16 Kbyte
  * 		Sector 2 0x0800 8000 - 0x0800 BFFF 16 Kbyte   
  *  		Sector 3 0x0800 C000 - 0x0800 FFFF 16 Kbyte     
  *     Sector 4 0x0801 0000 - 0x0801 FFFF 64 Kbyte		
  * -->          0x0801 F000 - 0x0801 FFFF  4 Kbyte		STORAGE
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "ParamDataFlash.h"
#include "string.h"

/* Global variables ---------------------------------------------------------*/
/* 32 bit aligned memory area */
#ifdef __ICCARM__
	#pragma location=0x08010000
	__root __no_init uint32_t extMem[8192];
#elif defined (__CC_ARM)
	__attribute__((zero_init)) static const uint32_t extMem[4096] __attribute__((at(0x08004000)));
	//__attribute__((zero_init)) static const uint32_t extMem[4096] __attribute__((section(".ARM.__at_0x08004000")));
	//__attribute__((zero_init)) uint32_t extMem[8192] __attribute__((section(".ARM.0x08010000")));
#elif defined (__GNUC__)
	uint32_t extMem[8192] __attribute__((section (".ExtMemRegion")));
#endif  
PARAMS_DEVICE params = {0};
PARAMS_DEVICE* pMemCurrParams;
uint8_t       fParamInit = 0;
int32_t      nDACdelay = 0;

#define MEMBLOCK_SIGN             0xABCDE025

#define SECTOR_MASK               ((uint32_t)0xFFFFFF07)

#define FLASH_TIMEOUT_VALUE       ((uint32_t)50000U)/* 50 s */

static HAL_StatusTypeDef FLASH_EraseSector(uint32_t FLASH_Sector, uint8_t VoltageRange);
static HAL_StatusTypeDef FLASH_ProgramByte(uint32_t Address, uint8_t Data);

void InitParams(void)
{
  params.MemBlockSign   = MEMBLOCK_SIGN;
	params._nodeid 				= 5;
}

// проверка есть ли блок параметров PARAMS_DEVICE в extMem
void ExistParams(void)
{
    uint16_t CountPD = 4096*4/sizeof(PARAMS_DEVICE);
    uint16_t i = 0;
	
    pMemCurrParams = (PARAMS_DEVICE* )extMem;
    
    for(i = 0; i < CountPD; ++i)
    {
        if(pMemCurrParams->MemBlockSign == MEMBLOCK_SIGN)
        {
            // найден блок с параметрами устройства
            memcpy(&params, pMemCurrParams, sizeof(PARAMS_DEVICE));
            break;
        }
        
        ++pMemCurrParams;
    }
    
    if(params.MemBlockSign == 0x0)
    {
        // блок параметров устройства не был найден
        // Unlock the Flash to enable the flash control register access 
        HAL_FLASH_Unlock();
        
        // Erase the user Flash area (area defined by extMem)
//        if(FLASH_EraseSector(FLASH_SECTOR_2, VOLTAGE_RANGE_3) == HAL_OK)
        {
//          if(FLASH_EraseSector(FLASH_SECTOR_3, VOLTAGE_RANGE_3) == HAL_OK)
          if(FLASH_EraseSector(FLASH_SECTOR_1, VOLTAGE_RANGE_3) == HAL_OK)
          {
              uint8_t* pAddressDist     = NULL;
              uint8_t* pAddressSource   = NULL;
              uint8_t* pPARAMS_END_ADDR = NULL; 
						
              InitParams();
              
              pAddressDist     = (uint8_t* )extMem;
              pAddressSource   = (uint8_t* )&params;
              pPARAMS_END_ADDR = ((uint8_t* )extMem) + sizeof(PARAMS_DEVICE); 

              // прописываем набор параметров по умолчанию
              while (pAddressDist < pPARAMS_END_ADDR)
              {
                if (FLASH_ProgramByte((uint32_t)pAddressDist, *pAddressSource) == HAL_OK)
                {
                  pAddressDist        = pAddressDist + 1;
                  pAddressSource      = pAddressSource + 1;
                }
                else
                {
                  /* Error occurred while writing data in Flash memory.
                     User can add here some code to deal with this error */
                  while (1)
                  {
                  }
                }
              }
          }
        }
        // Unlock the Flash to enable the flash control register access 
        HAL_FLASH_Lock();        
    }
}

void NewParams2Flash(void)
{
  // поиск структуры PARAMS_DEVICE
    uint16_t i = 0;
		uint8_t  j = 0;
		uint8_t* pAddressDist     = NULL;
		uint8_t* pAddressSource   = NULL;
		uint8_t* pPARAMS_END_ADDR = NULL; 
	
    uint16_t CountPD = 8192*4/sizeof(PARAMS_DEVICE);
    
    pMemCurrParams = (PARAMS_DEVICE* )extMem;
    
    for(i = 0; i < CountPD; ++i)
    {
        if(pMemCurrParams->MemBlockSign == MEMBLOCK_SIGN)
        {
            // найден блок с параметрами устройства
          HAL_FLASH_Unlock();
            // увеличиваем счетчик
            //params.DeviceID =  pMemCurrParams->DeviceID + 1;
            // прописываем найденный блок нулями
            pAddressDist = (uint8_t* )pMemCurrParams;
            
            for(j = 0; j < sizeof(PARAMS_DEVICE); ++j)
            {
              if (FLASH_ProgramByte((uint32_t)pAddressDist++, 0) != HAL_OK)
              {
                break;
              }
            }
            
            if((i + 1) == CountPD )
            {
              // подошли к границе блока данных,
              // необходимо очистить флэш и писать сначала блока
              pMemCurrParams = (PARAMS_DEVICE* )extMem;
              
              // Erase the user Flash area (area defined by extMem)
//              if(FLASH_EraseSector(FLASH_SECTOR_2, VOLTAGE_RANGE_3) == HAL_OK)
              if(FLASH_EraseSector(FLASH_SECTOR_4, VOLTAGE_RANGE_3) == HAL_OK)
              {
//                if(FLASH_EraseSector(FLASH_SECTOR_3, VOLTAGE_RANGE_3) == HAL_OK)
//                {
//                }
//                else
//                {
                    /*
                      Error occurred while sector erase.
                      User can add here some code to deal with this error.
                      SectorError will contain the faulty sector and then to know the code error on this sector,
                      user can call function 'HAL_FLASH_GetError()'
                    */
//                    while (1)
//                    {
//                    }
//                }
              }
              else
              {
                    /*
                      Error occurred while sector erase.
                      User can add here some code to deal with this error.
                      SectorError will contain the faulty sector and then to know the code error on this sector,
                      user can call function 'HAL_FLASH_GetError()'
                    */
                    while (1)
                    {
                    }
              }
            }
            else
              ++pMemCurrParams;
            
            // прописываем набор параметров в следующем блоке

            pAddressDist     = (uint8_t* )pMemCurrParams;
            pAddressSource   = (uint8_t* )&params;
            pPARAMS_END_ADDR = ((uint8_t* )pMemCurrParams) + sizeof(PARAMS_DEVICE); 
            
            while (pAddressDist < pPARAMS_END_ADDR)
            {
              if (FLASH_ProgramByte((uint32_t)pAddressDist, *pAddressSource) == HAL_OK)
              {
                pAddressDist        = pAddressDist + 1;
                pAddressSource      = pAddressSource + 1;
              }
              else
              {
                /* Error occurred while writing data in Flash memory.
                   User can add here some code to deal with this error */
                while (1)
                {
                }
              }
            }
            
            HAL_FLASH_Lock();

            break;
        }
        
        ++pMemCurrParams;
    }
}

void ReadFlash2Params()
{
    uint16_t CountPD = 8192*4/sizeof(PARAMS_DEVICE);
		uint16_t i = 0;
    
    pMemCurrParams = (PARAMS_DEVICE* )extMem;
    
    for(i = 0; i < CountPD; ++i)
    {
        if(pMemCurrParams->MemBlockSign == MEMBLOCK_SIGN)
        {
            // найден блок с параметрами устройства
            memcpy(&params, pMemCurrParams, sizeof(PARAMS_DEVICE));
            break;
        }
        
        ++pMemCurrParams;
    }
}

HAL_StatusTypeDef FLASH_EraseSector(uint32_t FLASH_Sector, uint8_t VoltageRange)
{
  uint32_t tmp_psize = 0x0;
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_FLASH_SECTOR(FLASH_Sector));
  assert_param(IS_VOLTAGERANGE(VoltageRange));
  
  if(VoltageRange == VOLTAGE_RANGE_1)
  {
     tmp_psize = FLASH_PSIZE_BYTE;
  }
  else if(VoltageRange == VOLTAGE_RANGE_2)
  {
    tmp_psize = FLASH_PSIZE_HALF_WORD;
  }
  else if(VoltageRange == VOLTAGE_RANGE_3)
  {
    tmp_psize = FLASH_PSIZE_WORD;
  }
  else
  {
    tmp_psize = FLASH_PSIZE_DOUBLE_WORD;
  }
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  { 
    CLEAR_BIT(FLASH->CR, FLASH_CR_PSIZE);
    FLASH->CR |= tmp_psize;
    CLEAR_BIT(FLASH->CR, FLASH_CR_SNB);
    FLASH->CR |= FLASH_CR_SER | (FLASH_Sector << POSITION_VAL(FLASH_CR_SNB));
    FLASH->CR |= FLASH_CR_STRT;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(HAL_FLASH_TIMEOUT_VALUE);
    
    /* if the erase operation is completed, disable the SER Bit */
    FLASH->CR &= (~FLASH_CR_SER);
    FLASH->CR &= SECTOR_MASK; 
  }
  /* Return the Erase Status */
  return status;
}

HAL_StatusTypeDef FLASH_ProgramByte(uint32_t Address, uint8_t Data)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  
  if(status == HAL_OK)
  {
    /* if the previous operation is completed, proceed to program the new data */
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_BYTE;
    FLASH->CR |= FLASH_CR_PG;
  
    *(__IO uint8_t*)Address = Data;
        
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(HAL_FLASH_TIMEOUT_VALUE);

    /* if the program operation is completed, disable the PG Bit */
    FLASH->CR &= (~FLASH_CR_PG);
  } 

  /* Return the Program Status */
  return status;
}

/******************************************************************************
*  Интерфейс
*
*******************************************************************************/
// назначение блока данных содержащий параметры устройства
void Storage_InitParams(void)
{
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGPERR);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR);
  ExistParams();
}

// запись параметров на флеш, вызывается после вызова Storage_InitParams
void Storage_SetParams(void)
{
  NewParams2Flash();
}

