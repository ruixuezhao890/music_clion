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
#include "StatusList.h"
#include "stdio.h"
#include "OSTask.h"
#include "key.h"
#include "lvgl.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"
#include "ui.h"
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
/* Definitions for MusicPlay */
osThreadId_t MusicPlayHandle;
const osThreadAttr_t MusicPlay_attributes = {
  .name = "MusicPlay",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for KeyScan */
osThreadId_t KeyScanHandle;
const osThreadAttr_t KeyScan_attributes = {
  .name = "KeyScan",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal4,
};
/* Definitions for lvglUpdata */
osThreadId_t lvglUpdataHandle;
const osThreadAttr_t lvglUpdata_attributes = {
  .name = "lvglUpdata",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal7,
};
/* Definitions for appUpdata */
osThreadId_t appUpdataHandle;
const osThreadAttr_t appUpdata_attributes = {
  .name = "appUpdata",
  .stack_size = 1024 * 8,
  .priority = (osPriority_t) osPriorityBelowNormal6,
};
/* Definitions for MusicStaute */
osEventFlagsId_t MusicStauteHandle;
const osEventFlagsAttr_t MusicStaute_attributes = {
  .name = "MusicStaute"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void MusicPlayTask(void *argument);
void KeyScanTask(void *argument);
void lvglUpdataTask(void *argument);
void appUpdataTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
  /* creation of MusicPlay */
  MusicPlayHandle = osThreadNew(MusicPlayTask, NULL, &MusicPlay_attributes);

  /* creation of KeyScan */
  KeyScanHandle = osThreadNew(KeyScanTask, NULL, &KeyScan_attributes);

  /* creation of lvglUpdata */
  lvglUpdataHandle = osThreadNew(lvglUpdataTask, NULL, &lvglUpdata_attributes);

  /* creation of appUpdata */
  appUpdataHandle = osThreadNew(appUpdataTask, NULL, &appUpdata_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of MusicStaute */
  MusicStauteHandle = osEventFlagsNew(&MusicStaute_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_MusicPlayTask */
/**
  * @brief  Function implementing the MusicPlay thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_MusicPlayTask */
void MusicPlayTask(void *argument)
{
  /* USER CODE BEGIN MusicPlayTask */
  /* Infinite loop */
  Music();
//    while (1){
//        osDelay(8);
//    }
  /* USER CODE END MusicPlayTask */
}

/* USER CODE BEGIN Header_KeyScanTask */
/**
* @brief Function implementing the KeyScan thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_KeyScanTask */
void KeyScanTask(void *argument)
{
  /* USER CODE BEGIN KeyScanTask */

  /* Infinite loop */
  memoryRes();
  /* USER CODE END KeyScanTask */
}

/* USER CODE BEGIN Header_lvglUpdataTask */
/**
* @brief Function implementing the lvglUpdata thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_lvglUpdataTask */
void lvglUpdataTask(void *argument)
{
  /* USER CODE BEGIN lvglUpdataTask */
  /* Infinite loop */
  for(;;)
  {
        lv_task_handler();
        osDelay(5);
  }
  /* USER CODE END lvglUpdataTask */
}

/* USER CODE BEGIN Header_appUpdataTask */
/**
* @brief Function implementing the appUpdata thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_appUpdataTask */
void appUpdataTask(void *argument)
{
  /* USER CODE BEGIN appUpdataTask */
  /* Infinite loop */
//    lv_init();
//    lv_port_disp_init();
//    lv_port_indev_init();
//    lv_fs_fatfs_init();
//    ui_Screen1_screen_init(lv_obj_create(NULL));
//    ui_Music_page_init(lv_scr_act());
//    while (1){
//        osDelay(6);
//    }
 APPManagerUpData();
  /* USER CODE END appUpdataTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

