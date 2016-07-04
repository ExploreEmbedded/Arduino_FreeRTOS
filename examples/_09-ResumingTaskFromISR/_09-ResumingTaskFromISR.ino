/***************************************************************************************************
                            ExploreEmbedded Copyright Notice    
****************************************************************************************************
 * File:   09-ResumingTaskFromISR
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

TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;
TaskHandle_t TaskHandle_4;

void setup()
{  
    Serial.begin(9600);
    Serial.println(F("In Setup function"));

    /* Use INT0(pin2) falling edge interrupt for resuming tasks */
    attachInterrupt(digitalPinToInterrupt(2), ExternalInterrupt, FALLING); 

  /* Create 3-tasks with priorities 2-4. Capture the Task details to respective handlers */
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, &TaskHandle_2);  
  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, &TaskHandle_3);
  xTaskCreate(MyTask4, "Task4", 100, NULL, 4, &TaskHandle_4);  
}


void loop()
{
    // Hooked to IDle task, it will run whenever CPU is idle
    Serial.println(F("Loop function"));
    delay(1000);
}


/* 
 *  Tasks are resumed every time a Falling edge interrupt is detected on PIN2.
 *  One task is resumed at a time, a counter is used to resume 3taks and after which no tasks are resumed.
 *  xTaskResumeFromISR() returns True if Context switch is required and accordingly we need to call portYIELD_FROM_ISR/taskYield(AVR).
 *  Serial data is printed in ISR only for demonstarting the control flow. This should not be done as it takes long time to send data on Serial port.
 *  Tasking to much ISR time will starve the other tasks or User application. 
 *  
 */
static void ExternalInterrupt()
{
    static int count=0;
    BaseType_t taskYieldRequired = 0;

    if(count<=3)
    {
        count++;
    }

    switch(count) // Resume one task at a time depending on count value
    {    
    case 1:
        Serial.println(F("ISR Resuming Task2"));
        taskYieldRequired = xTaskResumeFromISR(TaskHandle_2);
        Serial.println(F("Leaving ISR"));
        break;

    case 2:
        Serial.println(F("ISR Resuming Task3"));
        taskYieldRequired = xTaskResumeFromISR(TaskHandle_3);
        Serial.println(F("Leaving ISR"));
        break;

    case 3:
        Serial.println(F("ISR Resuming Task4"));
        taskYieldRequired = xTaskResumeFromISR(TaskHandle_4);
        Serial.println(F("Leaving ISR"));
        break;

    default:
        //DO nothing
        break;
    }

    if(taskYieldRequired == 1) // If the taskYield is reuiqred then trigger the same.
    {
        taskYIELD();
    }
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
    vTaskSuspend(NULL);         //Suspend Own Task

    Serial.println(F("Back in Task4, Deleting Itself"));
    vTaskDelete(TaskHandle_4);       
}