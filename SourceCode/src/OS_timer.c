#include"FreeRTOS.h"
#include "timers.h"

int TimerCounter = 0;
void Timer1_callback(TimerHandle_t* xTimer)
{
    TimerCounter++;
}