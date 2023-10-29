#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>

#include "dut_uart_task.h"

DutUartContext * _dut_context{nullptr};

void dut_uart_task(void * context)
{
    _dut_context = reinterpret_cast<DutUartContext *>(context);

    printf("task dut uart: start\n");
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1001));
        printf("dut: alive\n");
    }

}