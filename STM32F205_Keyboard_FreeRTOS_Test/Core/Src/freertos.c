/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Keyboard.h"
#include "Keyboard_dict.h"
#include "canfestival.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
int cnt = 0;
extern LED_Keyboard_t led_keyboard;
extern Keyboard_t keyboard;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId vLedONHandle;
osThreadId vKeyboardPollinHandle;
osThreadId vSendToCANHandle;
osThreadId vSendToUSBHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void vLedON_Handler(void const * argument);
void vKeyboardPolling_Handler(void const * argument);
void vSendToCAN_Handler(void const * argument);
void vSendToUSB_Handler(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of vLedON */
  osThreadDef(vLedON, vLedON_Handler, osPriorityRealtime, 0, 128);
  vLedONHandle = osThreadCreate(osThread(vLedON), NULL);

  /* definition and creation of vKeyboardPollin */
  osThreadDef(vKeyboardPollin, vKeyboardPolling_Handler, osPriorityHigh, 0, 128);
  vKeyboardPollinHandle = osThreadCreate(osThread(vKeyboardPollin), NULL);

  /* definition and creation of vSendToCAN */
  osThreadDef(vSendToCAN, vSendToCAN_Handler, osPriorityIdle, 0, 128);
  vSendToCANHandle = osThreadCreate(osThread(vSendToCAN), NULL);

  /* definition and creation of vSendToUSB */
  osThreadDef(vSendToUSB, vSendToUSB_Handler, osPriorityIdle, 0, 128);
  vSendToUSBHandle = osThreadCreate(osThread(vSendToUSB), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_vLedON_Handler */
/**
* @brief Function implementing the vLedON thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vLedON_Handler */
void vLedON_Handler(void const * argument)
{
  /* USER CODE BEGIN vLedON_Handler */
	static uint8_t i = 0;
  TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1 / portTICK_PERIOD_MS;
	xLastWakeTime = xTaskGetTickCount();
	
  for(;;)
  {
		LedColOFF(&led_keyboard, led_keyboard.btn[i]);
		LedRowOFF(&led_keyboard, 1 << i);
		if(++i > 4) 
			i = 0;
		LedRowON(&led_keyboard, 1 << i);
	
		if(led_keyboard.btn[i])
		{
			LedColON(&led_keyboard, led_keyboard.btn[i]);
		}

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
  /* USER CODE END vLedON_Handler */
}

/* USER CODE BEGIN Header_vKeyboardPolling_Handler */
/**
* @brief Function implementing the vKeyboardPollin thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vKeyboardPolling_Handler */
void vKeyboardPolling_Handler(void const * argument)
{
  /* USER CODE BEGIN vKeyboardPolling_Handler */
  /* Infinite loop */
  for(;;)
  {
		//KeyboardPollingStateFlow(&keyboard);
    osDelay(1);
  }
  /* USER CODE END vKeyboardPolling_Handler */
}

/* USER CODE BEGIN Header_vSendToCAN_Handler */
/**
* @brief Function implementing the vSendToCAN thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vSendToCAN_Handler */
void vSendToCAN_Handler(void const * argument)
{
  /* USER CODE BEGIN vSendToCAN_Handler */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vSendToCAN_Handler */
}

/* USER CODE BEGIN Header_vSendToUSB_Handler */
/**
* @brief Function implementing the vSendToUSB thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vSendToUSB_Handler */
void vSendToUSB_Handler(void const * argument)
{
  /* USER CODE BEGIN vSendToUSB_Handler */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vSendToUSB_Handler */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

