/***************************************************************************************************
                            ExploreEmbedded Copyright Notice    
****************************************************************************************************
 * File:   06-TaskPriorityChange
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the free rtos example to demonstarte task priority change.

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
TaskHandle_t TaskHandle_4;

void setup()
{
  
  Serial.begin(9600);
  Serial.println(F("In Setup function"));

  /* Create three tasks with priorities 1,2 and 3. Capture the Task details to respective handlers */
  xTaskCreate(MyTask1, "Task1", 120, NULL, 1, &TaskHandle_1);  

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
    Serial.println(F("Task1 with Priority:"));
    Serial.print(uxTaskPriorityGet(TaskHandle_1));
    Serial.print(F("Creating Task2"));
    
    xTaskCreate(MyTask2, "Task2", 100, NULL, 3, &TaskHandle_2);
    
    Serial.println(F("Task1 with Priority:"));
    Serial.print(uxTaskPriorityGet(TaskHandle_1));
    Serial.print(F(" Deleting All"));
    vTaskDelete(TaskHandle_2);   // Delete task2 and task4 using their handles
    vTaskDelete(TaskHandle_4); 
    vTaskDelete(TaskHandle_1);    // Delete the task using the TaskHandle_1
  }
}


/* Task2 with priority 2 */
static void MyTask2(void* pvParameters)
{
  while(1)
  {    
    Serial.println(F("Task2 Running, Creating Task4"));
    xTaskCreate(MyTask4, "Task4", 100, NULL, 4, &TaskHandle_4);
    
    Serial.println(F("Back in Task2, Deleting itself"));
    vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_2 can also be used)
  }
}



/* Task4 with priority 4 */
static void MyTask4(void* pvParameters)
{
    Serial.println(F("Task4 Running, Changing Priority of Task1 from 1-3"));
    vTaskPrioritySet(TaskHandle_1,5); //Change the priority of task1 to 5 which is hisghest

    while(1)
    {
      Serial.println(F("Back in Task4 "));
      vTaskDelay(100/portTICK_PERIOD_MS);   
    }
}