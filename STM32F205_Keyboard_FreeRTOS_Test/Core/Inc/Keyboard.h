#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f2xx_hal.h"

#define MAX_COL 8
#define MAX_ROW 5
#define KEYBOARD_ADDR (0x34 << 1)

/*KEYBOARD REGISTERS*/
#define REG_ADDR_CFG 				  	0x01
#define REG_ADDR_INT_STAT 	  	0x02
#define REG_ADDR_KEY_LCK_EC   	0x03
#define REG_ADDR_KEY_EVENT_A  	0x04
#define REG_ADDR_KEY_EVENT_B  	0x05
#define REG_ADDR_KEY_EVENT_C  	0x06
#define REG_ADDR_KEY_EVENT_D  	0x07
#define REG_ADDR_KEY_EVENT_E  	0x08
#define REG_ADDR_KEY_EVENT_F  	0x09
#define REG_ADDR_KEY_EVENT_G  	0x0A
#define REG_ADDR_KEY_EVENT_H  	0x0B
#define REG_ADDR_KEY_EVENT_I  	0x0C
#define REG_ADDR_KEY_EVENT_J  	0x0D

#define REG_ADDR_UNLOCK_1  			0x0F
#define REG_ADDR_UNLOCK_2		  	0x10

#define REG_ADDR_GPIO_INT_EN1		0x1A
#define REG_ADDR_GPIO_INT_EN2 	0x1B
#define REG_ADDR_GPIO_INT_EN3		0x1C

#define REG_ADDR_KP_GPIO1  			0x1D
#define REG_ADDR_KP_GPIO2 		 	0x1E
#define REG_ADDR_KP_GPIO3	  		0x1F

#define REG_ADDR_GPI_EM1  			0x20
#define REG_ADDR_GPI_EM2  			0x21
#define REG_ADDR_GPI_EM3	 	  	0x22

typedef struct
{
	uint16_t			btn[5];
	
	GPIO_TypeDef *row_gpio;
	GPIO_TypeDef *col_gpio;
}volatile LED_Keyboard_t;

typedef struct
{
	uint8_t			state_flow_state;
	uint8_t 		irq_event;
	uint8_t 		btn_cnt;
	uint8_t 		btn_state;
	uint8_t 		keyboard_ans_data;
	volatile uint64_t 	  full_data_btns;
}Keyboard_t;

typedef enum{
	IRQ_AVAIBLE_CHECK,
	IRQ_AMOUT_CHECK,
	BTN_STATE_CHECK,
	IRQ_CLEAR
}States;
	

void KeyboardInit(void);
void KeyboardPollingStateFlow(Keyboard_t *key);
void KeyboardRead(uint8_t *data_read);
void LedKeyboardInit(LED_Keyboard_t *key, GPIO_TypeDef *row_gpio, GPIO_TypeDef *col_gpio);
void LedRowON(LED_Keyboard_t *key,  uint16_t row);
void LedRowOFF(LED_Keyboard_t *key, uint16_t row);
void LedColON(LED_Keyboard_t *key,  uint16_t col);
void LedColOFF(LED_Keyboard_t *key, uint16_t col);
	
#ifdef __cplusplus
}
#endif

#endif /* __KEYBOARD_H */
