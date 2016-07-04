/***************************************************************************************************
                            ExploreEmbedded Copyright Notice    
****************************************************************************************************
 * File:   04-CreatingTaskFromOtherTask
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the free rtos example to demonstarte creating tasks from other tasks.

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
TaskHandle_t TaskHandle_4;

void setup()
{
  
  Serial.begin(9600);
  Serial.println(F("In Setup function"));

  /* Create two tasks with priorities 1 and 3. Capture the Task details to respective handlers */
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, &TaskHandle_1);  
  xTaskCreate(MyTask3, "Task3", 120, NULL, 3, &TaskHandle_3);

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
    Serial.println(F("Task1 Running and About to delete itself"));
    vTaskDelete(TaskHandle_1);    // Delete the task using the TaskHandle_1
  }
}


/* Task2 with priority 2 */
static void MyTask2(void* pvParameters)
{
  while(1)
  {    
    Serial.println(F("Task2 Running and About to delete itsel"));
    vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_2 can also be used)
  }
}


/* Task3 with priority 3 */
static void MyTask3(void* pvParameters)
{
  while(1)
  {    
    Serial.println(F("Task3 Running, Creating Task2 and Task4"));
    xTaskCreate(MyTask2, "Task2", 50, NULL, 2, &TaskHandle_2);
    xTaskCreate(MyTask4, "Task4", 100, NULL, 4, &TaskHandle_4);

    Serial.println(F("Back in Task3 and About to delete itself"));
    vTaskDelete(TaskHandle_3); 
  }
}


/* Task4 with priority 4 */
static void MyTask4(void* pvParameters)
{
  while(1)
  {    
    Serial.println(F("Task4 Running and About to delete itself"));
    vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_2 can also be used)
  }
}