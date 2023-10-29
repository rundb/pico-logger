#include "button_led_task.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"

ButtonLedContext * _button_led_context{nullptr};

void button_led_task(void *context)
{
    printf("task led-btn: start");
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}