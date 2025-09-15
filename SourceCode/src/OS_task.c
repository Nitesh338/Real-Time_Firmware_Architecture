#include"FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include"appl_main.h"

int Task1Var = 0;
int Task2Var = 0;
TickType_t TaskTickCount = 0;


void vTask1(void * pvparameter)
{
    const TickType_t CurrentTick = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(10);
    while(1)
    {
        Task1Var++;
        application_main();
        if(nvm_driverwriteStatus == b_BUSY)
        {
            nvm_write_status();
        }
        
        TaskTickCount = xTaskGetTickCount();
        (void)xTaskDelayUntil(&CurrentTick,xPeriod);
    }
}

void vTask2(void* pvparameter)
{
    while(1)
    {
        Task2Var++;
    }
}
