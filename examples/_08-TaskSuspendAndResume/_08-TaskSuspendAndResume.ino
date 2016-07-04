/***************************************************************************************************
                            ExploreEmbedded Copyright Notice    
****************************************************************************************************
 * File:   08-TaskSuspendAndResume
 * Version: 15.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: File contains the free rtos example to demonstarte task Suspend and Resume.

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

  /* Create 4-tasks with priorities 1-4. Capture the Task details to respective handlers */
  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, &TaskHandle_1);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, &TaskHandle_2);  
  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, &TaskHandle_3);
  xTaskCreate(MyTask4, "Task4", 100, NULL, 4, &TaskHandle_4); 
}


void loop()
{ // Hooked to Idle Task, will run when CPU is Idle
  Serial.println(F("Loop function"));
  delay(50);
}


/* Task1 with priority 1 */
static void MyTask1(void* pvParameters)
{
    Serial.println("Task1 Changing its priority to 5");
    vTaskPrioritySet(TaskHandle_1,5); //Now task1 is of highest priority
     
    Serial.println("Task1 Resuming Task2");
    vTaskResume(TaskHandle_2);
    
    Serial.println("Task1 Resuming Task3");
    vTaskResume(TaskHandle_3);

    Serial.println("Task1 Resuming Task4");
    vTaskResume(TaskHandle_4);

    Serial.println("Task1 Deleting Itself");
    vTaskDelete(TaskHandle_1);
}


/* Task2 with priority 2 */
static void MyTask2(void* pvParameters)
{        
    Serial.println(F("Task2, Deleting itself"));
    vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_2 can also be used)
}


/* Task3 with priority 3 */
static void MyTask3(void* pvParameters)
{
    Serial.println(F("Task3, Deleting Itself"));
    vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_3 can also be used)  
}


/* Task4 with priority 4 */
static void MyTask4(void* pvParameters)
{
    Serial.println(F("Task4 Running, Suspending all tasks"));
    vTaskSuspend(TaskHandle_2); //Suspend Task2/3
    vTaskSuspend(TaskHandle_3);
    vTaskSuspend(NULL); //Suspend Own Task

    Serial.println(F("Back in Task4, Deleting Itself"));
    vTaskDelete(TaskHandle_4);       
}