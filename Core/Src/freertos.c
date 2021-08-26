/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "chassis_task.h"
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

/* USER CODE END Variables */
osThreadId myTask01Handle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void chasssis_task(void const * argument);
void engineer_task(void const * argument);
void pid_calc_task(void const * argument);

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
  /* definition and creation of myTask01 */
  osThreadDef(myTask01, chasssis_task, osPriorityNormal, 0, 128);
  myTask01Handle = osThreadCreate(osThread(myTask01), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, engineer_task, osPriorityNormal, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, pid_calc_task, osPriorityNormal, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_chasssis_task */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_chasssis_task */
__weak void chasssis_task(void const * argument)
{
  /* USER CODE BEGIN chasssis_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END chasssis_task */
}

/* USER CODE BEGIN Header_engineer_task */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_engineer_task */
__weak void engineer_task(void const * argument)
{
  /* USER CODE BEGIN engineer_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END engineer_task */
}

/* USER CODE BEGIN Header_pid_calc_task */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_pid_calc_task */
void pid_calc_task(void const * argument)
{
  /* USER CODE BEGIN pid_calc_task */
	TickType_t pid_calc_t;  //用于vTaskDelayUntil初始化的变量
  const TickType_t TimeIncrement = pdMS_TO_TICKS(1); //将要延时的时间转化成系统tick
  pid_calc_t = xTaskGetTickCount(); //获取当前系统tick
  /* Infinite loop */
  while(1){
		
			ele_left_position_pid.get = RM3508_Elevator_Data_L.Resolve_Position;
			PidCalculate(&ele_left_position_pid);
			ele_left_speed_pid.set = ele_left_position_pid.out;
			ele_left_speed_pid.get = RM3508_Elevator_Data_L.RollSpeed;
			PidCalculate(&ele_left_speed_pid);
			
			ele_right_position_pid.get = RM3508_Elevator_Data_R.Resolve_Position;
			PidCalculate(&ele_right_position_pid);
			ele_right_speed_pid.set = ele_right_position_pid.out;
			ele_right_speed_pid.get = RM3508_Elevator_Data_R.RollSpeed;
			PidCalculate(&ele_right_speed_pid);
				
			snatch_position_pid.get = RM3508_Snatch_Data.Resolve_Position;
			PidCalculate(&snatch_position_pid);
			snatch_speed_pid.set = snatch_position_pid.out;
			snatch_speed_pid.get = RM3508_Snatch_Data.RollSpeed;
			PidCalculate(&snatch_speed_pid);
			
			if(uuid_t.init_uuid == 2){
				
				sendMotorCurrent(&hcan1,0x1ff,ele_left_speed_pid.out,ele_right_speed_pid.out,0,0);	
																			
				sendMotorCurrent(&hcan2,0x200,snatch_speed_pid.out,0,0,0);
			}
			
			vTaskDelayUntil(&pid_calc_t ,TimeIncrement);
  }
  /* USER CODE END pid_calc_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
