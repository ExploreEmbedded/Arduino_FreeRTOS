/***************************************************************************************************
                                              ExploreEmbedded Copyright Notice 
****************************************************************************************************
 * File:   main.c
 * Version: 16.0
 * Author: ExploreEmbedded
 * Website: http://www.exploreembedded.com/wiki
 * Description: Program to demonstrate the Binary semaphore usage and priority inversion 

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

TaskHandle_t LPT_TaskHandle;
TaskHandle_t MPT_TaskHandle;
TaskHandle_t HPT_TaskHandle;

SemaphoreHandle_t binSemaphore_A = NULL;

#define printMsg(taskhandle,str)  {\
        Serial.print(F("Priority "));\  // Print task priority 
        Serial.print(uxTaskPriorityGet(taskhandle));\
        Serial.print(F(" : "));\
        Serial.println(F(str));\        // Print user string
}

void setup()
{  
    Serial.begin(9600);
    Serial.println(F("In Setup function, Creating Binary Semaphore"));

    vSemaphoreCreateBinary(binSemaphore_A);  /* Create binary semaphore */

    if(binSemaphore_A != NULL)
    {
        Serial.println(F("Creating low priority task"));
        xTaskCreate(LPT_Task, "LPT_Task", 100, NULL, 1, &LPT_TaskHandle);
    }
    else
    {
        Serial.println(F("Failed to create Semaphore"));
    }
}


void loop()
{ // Hooked to Idle Task, will run when CPU is Idle
    Serial.println(F("Loop function"));
    delay(50);
}


/*LPT: Low priority task*/
void LPT_Task(void* pvParameters)
{
    printMsg(LPT_TaskHandle,"LPT_Task Acquiring semaphore"); 
    xSemaphoreTake(binSemaphore_A,portMAX_DELAY);

    printMsg(LPT_TaskHandle,"LPT_Task Creating HPT"); 
    xTaskCreate(HPT_Task, "HPT_Task", 100, NULL, 3, &HPT_TaskHandle); 


    printMsg(LPT_TaskHandle,"LPT_Task Creating MPT");  
    xTaskCreate(MPT_Task, "MPT_Task", 100, NULL, 2, &MPT_TaskHandle); 

    printMsg(LPT_TaskHandle,"LPT_Task Releasing Semaphore");
    xSemaphoreGive(binSemaphore_A);

    printMsg(LPT_TaskHandle,"LPT_Task Finally Exiting");
    vTaskDelete(LPT_TaskHandle);
}


/*MPT: Medium priority task*/
void MPT_Task(void* pvParameters)
{
    printMsg(MPT_TaskHandle,"MPT_Task Done and about to exit");
    vTaskDelete(MPT_TaskHandle);
}


/*MPT: High priority task*/
void HPT_Task(void* pvParameters)
{
    printMsg(HPT_TaskHandle,"HPT_Task Trying to Acquire the semaphore");
    xSemaphoreTake(binSemaphore_A,portMAX_DELAY);

    printMsg(HPT_TaskHandle,"HPT_Task Acquired the semaphore");    

    printMsg(HPT_TaskHandle,"HPT_Task Releasing the semaphore");
    xSemaphoreGive(binSemaphore_A);    

    printMsg(HPT_TaskHandle,"HPT_Task About to Exit");
    vTaskDelete(HPT_TaskHandle);
}


