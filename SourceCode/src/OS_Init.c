#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "OS_task.h"
#include "OS_timer.h"
#include"appl_main.h"


#define Timer1_period   pdMS_TO_TICKS(500)

int Init()
{
    BaseType_t returnx;
    TimerHandle_t TimerHandle;
    nvm_init();
    returnx = xTaskCreate(vTask1,"T1",configMINIMAL_STACK_SIZE,NULL,1,NULL);
    returnx = xTaskCreate(vTask2,"T2",configMINIMAL_STACK_SIZE,NULL,1,NULL);
    TimerHandle = xTimerCreate("Timer1",(TickType_t)Timer1_period, (BaseType_t)pdTRUE,(void *)0,(TimerCallbackFunction_t)&Timer1_callback);
    returnx = xTimerStart(TimerHandle, 0);
    vTaskStartScheduler();

    for (;;);
    return 0;
}
