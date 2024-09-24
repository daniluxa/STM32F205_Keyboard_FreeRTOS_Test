#include "Keyboard.h"
#include "gpio.h"
#include "i2c.h"
#include "Keyboard_dict.h"

void KeyboardInit(void)
{
	uint8_t irq_init_data = 0x95;
	uint8_t row_init_data = 0x3e;
	uint8_t col_0_7_init_data = 0xff;
	uint8_t col_8_9_init_data = 0;
	
	HAL_I2C_Mem_Write(&hi2c1, KEYBOARD_ADDR, REG_ADDR_CFG, I2C_MEMADD_SIZE_8BIT, &irq_init_data, sizeof(irq_init_data), HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, KEYBOARD_ADDR, REG_ADDR_KP_GPIO1, I2C_MEMADD_SIZE_8BIT, &row_init_data, sizeof(row_init_data), HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, KEYBOARD_ADDR, REG_ADDR_KP_GPIO2, I2C_MEMADD_SIZE_8BIT, &col_0_7_init_data, sizeof(col_0_7_init_data), HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, KEYBOARD_ADDR, REG_ADDR_KP_GPIO3, I2C_MEMADD_SIZE_8BIT, &col_8_9_init_data, sizeof(col_8_9_init_data), HAL_MAX_DELAY);
}

void KeyboardPollingStateFlow(Keyboard_t *key)
{
	uint8_t clear_irq = 0x07;
	switch(key->state_flow_state)
	{
		case IRQ_AVAIBLE_CHECK:
			HAL_I2C_Mem_Read(&hi2c1, KEYBOARD_ADDR, REG_ADDR_INT_STAT, I2C_MEMADD_SIZE_8BIT, &key->irq_event, 1, HAL_MAX_DELAY);
			if(key->irq_event & 0x07)
				key->state_flow_state = IRQ_AMOUT_CHECK;
			break;
		case IRQ_AMOUT_CHECK:
			HAL_I2C_Mem_Read(&hi2c1, KEYBOARD_ADDR, REG_ADDR_KEY_LCK_EC, I2C_MEMADD_SIZE_8BIT, &key->btn_cnt, 1, HAL_MAX_DELAY);
			key->state_flow_state = BTN_STATE_CHECK;
			break;
		case BTN_STATE_CHECK:
			if(key->btn_cnt & 0x0F)
				if(key->btn_cnt > 0)
					key->btn_cnt--;
				HAL_I2C_Mem_Read(&hi2c1, KEYBOARD_ADDR, REG_ADDR_KEY_EVENT_A, I2C_MEMADD_SIZE_8BIT, &key->keyboard_ans_data, 1, HAL_MAX_DELAY);
				if(key->keyboard_ans_data & 0x80)
				{
					key->keyboard_ans_data &= ~0x80;
					key->full_data_btns |= ((uint64_t)1 << (key->keyboard_ans_data - 1));
					Buttons_State[0] = key->full_data_btns & 0xFFFFFFFF;
					Buttons_State[1] = (key->full_data_btns >> 32) & 0xFFFFFFFF;
				}
				else if(key->keyboard_ans_data != 0)
				{
					key->full_data_btns &= ~((uint64_t)1 << (key->keyboard_ans_data - 1));
					Buttons_State[0] = key->full_data_btns & 0x00000000FFFFFFFF;
					Buttons_State[1] = (key->full_data_btns >> 32) & 0xFFFFFFFF;
				}
			else
				key->state_flow_state = IRQ_CLEAR;
				key->btn_cnt = 0;
				key->irq_event = 0;
			break;
		case IRQ_CLEAR:
			HAL_I2C_Mem_Write(&hi2c1, KEYBOARD_ADDR, REG_ADDR_INT_STAT, I2C_MEMADD_SIZE_8BIT, &clear_irq, 1, HAL_MAX_DELAY);
			key->state_flow_state = IRQ_AVAIBLE_CHECK;
			break;	
	}
}

void LedKeyboardInit(LED_Keyboard_t *key, GPIO_TypeDef *row_gpio, GPIO_TypeDef *col_gpio)
{
	key->row_gpio = row_gpio;
	key->col_gpio = col_gpio;
}

void LedRowON(LED_Keyboard_t *key, uint16_t row)
{
	HAL_GPIO_WritePin(key->row_gpio, row, GPIO_PIN_SET);
}

void LedRowOFF(LED_Keyboard_t *key, uint16_t row)
{
	HAL_GPIO_WritePin(key->row_gpio, row, GPIO_PIN_RESET);
}

void LedColON(LED_Keyboard_t *key, uint16_t col)
{
	HAL_GPIO_WritePin(key->col_gpio, col, GPIO_PIN_RESET);
}

void LedColOFF(LED_Keyboard_t *key, uint16_t col)
{
	HAL_GPIO_WritePin(key->col_gpio, col, GPIO_PIN_SET);
}
