#pragma once

#include "FreeRTOS.h"
#include "queue.h"

struct DutUartContext
{
    QueueHandle_t rx_data_handle{nullptr};
};

void dut_uart_task(void * context);
