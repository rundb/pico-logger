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

#define SDCARD_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define UART_TASK_PRIO (tskIDLE_PRIORITY + 1)

#define SDCARD_STACK_SIZE (2048)

MemoryContext memory_context;

TaskHandle_t sdcard_task_handle;
StackType_t sdcard_stack[SDCARD_STACK_SIZE] {0UL};

int main() {
    // stdio_init_all();
    stdio_uart_init_full(uart1, 115200, 20, 21);
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

