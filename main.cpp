#include <stdio.h>
//
#include "pico/stdlib.h"
#include "rtc.h"
#include "pico/stdio_uart.h"
//
#include "hw_config.h"

#include "FreeRTOS.h"
#include "task.h"

#include "sdcard_task.h"
#include "dut_uart_task.h"

#define SDCARD_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define DUT_TASK_PRIO (tskIDLE_PRIORITY + 1)

#define SDCARD_STACK_SIZE (2048)
#define DUT_STACK_SIZE (512)

#define STDIO_UART_TX_PIN (20)
#define STDIO_UART_RX_PIN (21)
#define STDIO_UART_BAUDRATE (115200)

MemoryContext memory_context;
DutUartContext dut_context;

TaskHandle_t sdcard_task_handle;
TaskHandle_t dut_task_handle;
StackType_t sdcard_stack[SDCARD_STACK_SIZE] {0UL};

int main() {
    // stdio_init_all();
    stdio_uart_init_full(uart1, STDIO_UART_BAUDRATE, STDIO_UART_TX_PIN, STDIO_UART_RX_PIN);
    time_init();

    // puts("Hello, world!");

    const auto sdcard_task_result = xTaskCreate(
        sdcard_thread,
        "SDC", 
        SDCARD_STACK_SIZE, 
        &memory_context, 
        SDCARD_TASK_PRIO, 
        &sdcard_task_handle
    );
    if (pdPASS != sdcard_task_result)
    {
        printf("failed to create task sdcard, result=%d\n");
    }

    const auto dut_task_result = xTaskCreate(
        dut_uart_task,
        "DUT", 
        DUT_STACK_SIZE, 
        &dut_context, 
        DUT_TASK_PRIO, 
        &dut_task_handle
    );
    if (pdPASS != dut_task_result)
    {
        printf("failed to create task dut, result=%d\n");
    }


    vTaskStartScheduler();
    puts("should be unreachable");
    for (;;);
}

void vApplicationTickHook (void)
{
};

void vApplicationStackOverflowHook(TaskHandle_t Task, char *pcTaskName)
{
  panic("stack overflow (not the helpful kind) for %s\n", *pcTaskName);
}

void vApplicationMallocFailedHook(void)
{
  panic("Malloc Failed\n");
};

