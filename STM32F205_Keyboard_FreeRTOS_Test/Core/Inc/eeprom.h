#ifndef __EEPROM_H
#define __EEPROM_H
#include "main.h"

	int eeprom_read (uint32_t addr, void *data, uint32_t size);
	int eeprom_write(uint32_t addr, void *data, uint32_t size);

#endif
