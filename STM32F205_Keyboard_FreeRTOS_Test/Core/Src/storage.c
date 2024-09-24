/*
**  storage.c
*/

#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "Keyboard_dict.h"
#include "print_ch.h"
#include "eeprom.h"
#include "storage.h"
#include "ParamDataFlash.h"

#define MAGIC_KEY 		0xABCDE014//0x1235ABCD
#define ADDR_MAGIC 		0
#define SIZE_MAGIC 		4
#define ADDR_NODEID		4
#define SIZE_NODEID 	1
#define ADDR_DEV_MAP	64

int stg_IsMagic(void)
{
	ExistParams();
	return 1;
//	uint32_t magic = 0;
//	
//	eeprom_read (ADDR_MAGIC, &magic, SIZE_MAGIC);
//	if(MAGIC_KEY == magic)
//		return 1;
//	else
//		return 0;
	
}

void stg_SetData(uint16_t addr_off, void* data, uint16_t size)
{
	return;
	// если на плате есть eeprom, то используется след алгоритм
//	// пишем данные в EEPROM
//		uint32_t magic = 0;
//		
//		// сохраняем значение 
//		eeprom_read (ADDR_MAGIC, &magic, SIZE_MAGIC);
//		if(MAGIC_KEY == magic)
//		{
//			eeprom_write(addr_off, data, size);
//		}
//		else
//		{
//			magic = MAGIC_KEY;
//			eeprom_write(ADDR_MAGIC, &magic, SIZE_MAGIC);
//			eeprom_write(addr_off, data, size);
//		}	
}

void stg_GetData(uint16_t addr_off, void* data, uint16_t size)
{
	return;
	// если на плате есть eeprom, то используется след алгоритм
//	// пишем данные в EEPROM
//	uint32_t magic = 0;
//	
//	// сохраняем значение 
//	eeprom_read (ADDR_MAGIC, &magic, SIZE_MAGIC);
//	if(MAGIC_KEY == magic)
//	{
//		eeprom_read(addr_off, data, size);
//	}
}

void stg_SetValue(UNS16 wIndex, UNS8 bSubindex)
{
	return;
	// если на плате есть eeprom, то используется след алгоритм, при использовании flash-памяти сохраниение параметров can-open не работает
//	UNS32  data=0;
//	UNS32  size=sizeof(data);
//	UNS8 *pDataType;
//	
//	UNS32 ret = _getODentry( &_Data, wIndex, bSubindex, &data, &size, pDataType, 0, 0);
//	if( ret == OD_SUCCESSFUL)
//	{
//		print_ch(CH_TTY1,"\t Value:%u\n\r",	data);
//		print_ch(CH_TTY1,"\t size:%u\n\r",	size);
//		int addr_off=0;
//		for(int i=0; i < *(&_Data)->ObjdictSize; i++)
//		{
//			//print_ch(CH_TTY1,"\n\r");
//			//print_ch(CH_TTY1,"\tIndex:0x%X size:%u\n\r",d->objdict[i].index,d->objdict[i].bSubCount);
//			for(int ii=0; ii < (&_Data)->objdict[i].bSubCount; ii++)
//			{
//				//print_ch(CH_TTY1,"\t\tSub:0x%X size:%u\n\r", ii,d->objdict[i].pSubindex[ii].size);
//				if(((&_Data)->objdict[i].index == wIndex) && (ii == bSubindex)) break;
//				addr_off+=(&_Data)->objdict[i].pSubindex[ii].size;
//			}

//			if((&_Data)->objdict[i].index == wIndex) break;
//		}
//		print_ch(CH_TTY1,"\n\rAddr offset:%u\n\r",	addr_off);
//			
//		
//		// сохраняем значение 
//		stg_SetData(ADDR_DEV_MAP + addr_off, &data, size);		
//	}

//	return;
}

// пишем всю карту памяти прибора в EEPROM
void stg_SetDEVmap(CO_Data* d)
{
	return;
	// если на плате есть eeprom, то используется след алгоритм
//	uint32_t magic = 0;
//	
//	stg_SetNodeID(3);
//	
//	magic = MAGIC_KEY;
//	eeprom_write(ADDR_MAGIC, &magic, SIZE_MAGIC);
//	
//	UNS32 addr_off=0;
//	for(int i=0; i < *d->ObjdictSize; i++){
//		for(int ii=0; ii < d->objdict[i].bSubCount; ii++){
//			stg_SetData(ADDR_DEV_MAP + addr_off, d->objdict[i].pSubindex[ii].pObject, d->objdict[i].pSubindex[ii].size);
//			stg_GetData(ADDR_DEV_MAP + addr_off, d->objdict[i].pSubindex[ii].pObject, d->objdict[i].pSubindex[ii].size);
//			//print_ch(CH_TTY1,"\t\tSub:0x%X size:%u\n\r", ii,d->objdict[i].pSubindex[ii].size);
//			addr_off+=d->objdict[i].pSubindex[ii].size;
//		}		
//	}
}

void stg_GetDEVmap(CO_Data* d)
{
	return;
	// если на плате есть eeprom, то используется след алгоритм
//	
//	UNS32 addr_off=0;
//	for(int i=0; i < *d->ObjdictSize; i++){
//		//print_ch(CH_TTY1,"\n\r");
//		//print_ch(CH_TTY1,"\tIndex:0x%X size:%u\n\r",d->objdict[i].index,d->objdict[i].bSubCount);
//		for(int ii=0; ii < d->objdict[i].bSubCount; ii++){
//			stg_GetData(ADDR_DEV_MAP + addr_off, d->objdict[i].pSubindex[ii].pObject, d->objdict[i].pSubindex[ii].size);
//			//print_ch(CH_TTY1,"\t\tSub:0x%X size:%u\n\r", ii,d->objdict[i].pSubindex[ii].size);
//			addr_off+=d->objdict[i].pSubindex[ii].size;
//		}		
//	}
//	
}

//void stg_SetNodeID(UNS8 nodeid)
//{
//	uint32_t magic = 0;
//	UNS8 _nodeid   = nodeid;
//	
//	// сохраняем значение nodeID 
//	eeprom_read (ADDR_MAGIC, &magic, SIZE_MAGIC);
//	if(MAGIC_KEY == magic)
//	{
//		eeprom_write(ADDR_NODEID, &_nodeid, SIZE_NODEID);
//	}
//	else
//	{
//		magic = MAGIC_KEY;
//		eeprom_write(ADDR_MAGIC, &magic, SIZE_MAGIC);
//		eeprom_write(ADDR_NODEID, &_nodeid, SIZE_NODEID);		
//	}
//}

void stg_SetNodeID(UNS8 nodeid){
	params._nodeid = nodeid;
	NewParams2Flash();
	
	// если на плате есть eeprom, то используется след алгоритм
//	uint32_t magic = 0;
//	UNS8 _nodeid   = nodeid;
//	
//	// сохраняем значение nodeID 
//	eeprom_read (ADDR_MAGIC, &magic, SIZE_MAGIC);
//	if(MAGIC_KEY == magic)
//	{
//		eeprom_write(ADDR_NODEID, &_nodeid, SIZE_NODEID);
//	}
//	else
//	{
//		magic = MAGIC_KEY;
//		eeprom_write(ADDR_MAGIC, &magic, SIZE_MAGIC);
//		eeprom_write(ADDR_NODEID, &_nodeid, SIZE_NODEID);		
//	}
}

//UNS8 stg_GetNodeID(void)
//{
//	uint32_t magic = 0;
//	UNS8 _nodeid   = 0xFF;
//	
//	eeprom_read (ADDR_MAGIC, &magic, SIZE_MAGIC);
//	if(MAGIC_KEY == magic)
//	{
//		// читаем значение nodeID 
//		eeprom_read (ADDR_NODEID, &_nodeid, SIZE_NODEID);
//		if((_nodeid < 1) || (_nodeid > 127))
//			_nodeid = 3;
//	}
//	else
//	{
//		_nodeid = 3;
//	}
//	return _nodeid;
//}

UNS8 stg_GetNodeID(void)
{
	UNS8 _nodeid   = 0xFF;
	ReadFlash2Params();
	//_nodeid = *(uint8_t*)0x08010000;
	_nodeid = params._nodeid;
	return _nodeid;
	
	// если на плате есть eeprom, то используется след алгоритм
	
//	uint32_t magic = 0;
//	UNS8 _nodeid   = 0xFF;
	
//	eeprom_read (ADDR_MAGIC, &magic, SIZE_MAGIC);
//	if(MAGIC_KEY == magic)
//	{
//		// читаем значение nodeID 
//		eeprom_read (ADDR_NODEID, &_nodeid, SIZE_NODEID);
//		if((_nodeid < 1) || (_nodeid > 127))
//			_nodeid = 3;
//	}
//	else
//	{
//		_nodeid = 3;
//	}
//	return _nodeid;
}
