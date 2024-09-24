#include "eeprom.h"
#include "stdio.h"

#include "string.h"
#include "stm32f2xx_hal.h"


#define EEPROM_ADDR			(0x50 << 1)
#define EEPROM_SIZE_IN_BYTE	( 32 * 1024) 
#define EEPROM_PAGE_SIZE	64 
	

int eeprom_write(uint32_t addr, void *data, uint32_t size)
{
	return 1;
}


int eeprom_read (uint32_t addr, void *data, uint32_t size)
{
	return 1;
}