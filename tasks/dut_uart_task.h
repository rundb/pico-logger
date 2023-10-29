#pragma once

#include "FreeRTOS.h"
#include "queue.h"

#define DUT_RX_DATA_MAX_DATA_SIZE (64)

struct DutRxData
{
    uint32_t size{0};
    uint8_t data[DUT_RX_DATA_MAX_DATA_SIZE] {0};
};

struct DutUartContext
{
    QueueHandle_t rx_data_handle{nullptr};
};

void dut_uart_task(void * context);
