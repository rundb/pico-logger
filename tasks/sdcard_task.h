#pragma once

#include "FreeRTOS.h"
#include "queue.h"

struct MemoryContext
{
    QueueHandle_t command_handle{nullptr};
    QueueHandle_t data_handle{nullptr};
};

void sdcard_thread(void *context);
