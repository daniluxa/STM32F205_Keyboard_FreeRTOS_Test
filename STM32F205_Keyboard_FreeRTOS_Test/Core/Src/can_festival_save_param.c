#include "data.h"
#include "Keyboard_dict.h"
#include "print_ch.h"
#include "storage.h"

#define DBG_MODE	


 const indextable * scanIndexOD (CO_Data* d, UNS16 wIndex, UNS32 *errorCode, ODCallback_t **Callback);
 void _storeODSubIndex (CO_Data* d, UNS16 wIndex, UNS8 bSubindex)
{
	UNS32  temp;
	 
	 #ifdef DBG_MODE
		print_ch(CH_TTY1,"%s(Index:0x%X Sub:0x%X)\n\r",__FUNCTION__,wIndex, bSubindex);
	 #endif
	stg_SetValue(wIndex, bSubindex);

	temp=0;
	for(int i=0; i < *d->ObjdictSize; i++)
	{
		for(int ii=0; ii < d->objdict[i].bSubCount; ii++){
			temp+=d->objdict[i].pSubindex[ii].size;
		}

		if(d->objdict[i].index == wIndex) break;
	}
	
	print_ch(CH_TTY1,"\n\rAddr offset:%u\n\r",	temp);
}

