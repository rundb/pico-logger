#pragma once

#include "FreeRTOS.h"
#include "queue.h"

struct ButtonLedContext
{
    QueueHandle_t command_handle{nullptr};
};

void button_led_task(void *context);
