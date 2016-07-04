/***************************************************************************************************
                            ExploreEmbedded Copyright Notice    
****************************************************************************************************
 * File:   10-ReadTaskInfo
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

char ptrTaskList[250];

void setup()
{  
    Serial.begin(9600);
    Serial.println(F("In Setup function"));

    /* Use INT0(pin2) falling edge interrupt for resuming tasks */
    attachInterrupt(digitalPinToInterrupt(2), ExternalInterrupt, FALLING); 

    /* Create a task with priority 3. Capture the Task details to its handler*/
    xTaskCreate(MyTask1, "Task1", 120, NULL, 1, &TaskHandle_1);
    xTaskCreate(MyTask2, "Task2", 120, NULL, 2, &TaskHandle_2);  
    xTaskCreate(MyTask3, "Task3", 120, NULL, 3, &TaskHandle_3);
}


void loop()
{
    // Hooked to IDle task, it will run whenever CPU is idle
    Serial.println(F("Loop function"));
    delay(1000);
}


/* 
 *  Task info(state, Priority, Stack available) is read immediately the switch is pressed and sent on Serial Port
 *  Serial data is printed in ISR for demonstarting the control flow. This should not be done as it takes long time to send data on Serial port.
 *  Taking to much ISR time will starve the other tasks or User application.  *  
 */
static void ExternalInterrupt()
{
    vTaskList(ptrTaskList);
    Serial.println(F("**********************************"));
    Serial.println(F("Task  State   Prio    Stack    Num")); 
    Serial.println(F("**********************************"));
    Serial.print(ptrTaskList);
    Serial.println(F("**********************************"));
}



/* Task1 with priority 1 */
static void MyTask1(void* pvParameters)
{
    while(1)
    {
        Serial.println(F("Task1 Running"));
        delay(200);
        vTaskDelay(250/portTICK_PERIOD_MS);
    }
}


/* Task2 with priority 2 */
static void MyTask2(void* pvParameters)
{
    while(1)
    {    
        Serial.println(F("Task2 Running"));
        delay(200);
        vTaskDelay(300/portTICK_PERIOD_MS);
    }
}


/* Task3 with priority 3 */
static void MyTask3(void* pvParameters)
{
    char suspendFlag=0;
    while(1)
    {    
        Serial.println(F("Task3 Running"));

        if(suspendFlag==1)                 //Task1 is suspended and resumed alternatively whenever Task3 runs.
        {
            vTaskSuspend(TaskHandle_1);
            suspendFlag = 0;
        }
        else
        {
            vTaskResume(TaskHandle_1);
            suspendFlag = 1;
        }
        delay(200);
        vTaskDelay(400/portTICK_PERIOD_MS);
    }
}