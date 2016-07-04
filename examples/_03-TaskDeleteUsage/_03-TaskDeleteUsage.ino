/***************************************************************************************************
                            ExploreEmbedded Copyright Notice    
****************************************************************************************************
 * File:   03-TaskDeleteUsage
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the free rtos example to demonstarte the task delete.

This code has been developed and tested on ExploreEmbedded boards.  
We strongly believe that the library works on any of development boards for respective controllers. 
Check this link http://www.exploreembedded.com/wiki for awesome tutorials on 8051,PIC,AVR,ARM,Robotics,RTOS,IOT.
ExploreEmbedded invests substantial time and effort developing open source HW and SW tools, to support consider 
buying the ExploreEmbedded boards.
 
The ExploreEmbedded libraries and examples are licensed under the terms of the new-bsd license(two-clause bsd license).
See also: http://www.opensource.org/licenses/bsd-license.php

EXPLOREEMBEDDED DISCLAIMS ANY KIND OF HARDWARE FAILURE RESULTING OUT OF USAGE OF LIBRARIES, DIRECTLY OR
INDIRECTLY. FILES MAY BE SUBJECT TO CHANGE WITHOUT PRIOR NOTICE. THE REVISION HISTORY CONTAINS THE INFORMATION 
RELATED TO UPDATES.
 

Permission to use, copy, modify, and distribute this software and its documentation for any purpose
and without fee is hereby granted, provided that this copyright notices appear in all copies 
and that both those copyright notices and this permission notice appear in supporting documentation.
**************************************************************************************************/

#include <Arduino_FreeRTOS.h>

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;

void setup()
{
  
  Serial.begin(9600);
  Serial.println(F("In Setup function"));

  /* Create three tasks with priorities 1,2 and 3. Capture the Task details to respective handlers */
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, &TaskHandle_1);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, &TaskHandle_2);
  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, &TaskHandle_3);
}


void loop()
{
  // Hooked to IDle task, it will run whenever CPU is idle
  Serial.println(F("Loop function"));
  delay(50);
}


/* Task1 with priority 1 */
static void MyTask1(void* pvParameters)
{
  while(1)
  {
    Serial.println(F("Task1 Running"));
    vTaskDelay(100/portTICK_PERIOD_MS);

    Serial.println(F("Back in Task1 and About to delete itself"));
    vTaskDelete(TaskHandle_1);    // Delete the task using the TaskHandle_1
  }
}


/* Task2 with priority 2 */
static void MyTask2(void* pvParameters)
{
  while(1)
  {    
    Serial.println(F("Task2 Running"));
    vTaskDelay(150/portTICK_PERIOD_MS);

    Serial.println(F("Back in Task2 and About to delete itself"));
    vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_2 can also be used)
  }
}


/* Task3 with priority 3 */
static void MyTask3(void* pvParameters)
{
  while(1)
  {    
    Serial.println(F("Task3 Running"));
    vTaskDelay(150/portTICK_PERIOD_MS);

    Serial.println(F("Back in Task3 and About to delete itself"));
    vTaskDelete(TaskHandle_3); 
  }
}

