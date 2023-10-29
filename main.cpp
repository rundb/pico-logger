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
#include "button_led_task.h"

#define SDCARD_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define DUT_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define BUTTON_LED_TASK_PRIO (tskIDLE_PRIORITY + 1)

#define SDCARD_STACK_SIZE (2048)
#define DUT_STACK_SIZE (512)
#define BUTTON_LED_STACK_SIZE (configMINIMAL_STACK_SIZE)

#define STDIO_UART_TX_PIN (20)
#define STDIO_UART_RX_PIN (21)
#define STDIO_UART_BAUDRATE (115200)

#define DUT_UART_DATA_QUEUE_DEPTH (3)
#define SD_CARD_CMD_DATA_QUEUE_DEPTH (1)

MemoryContext memory_context;
DutUartContext dut_context;
ButtonLedContext btn_led_context;

TaskHandle_t sdcard_task_handle;
TaskHandle_t dut_task_handle;
TaskHandle_t btn_led_task_handle;

QueueHandle_t dut_uart_data_queue;
QueueHandle_t memory_cmd_queue;

int main() {
    // stdio_init_all();
    stdio_uart_init_full(uart1, STDIO_UART_BAUDRATE, STDIO_UART_TX_PIN, STDIO_UART_RX_PIN);
    time_init();

    dut_uart_data_queue = xQueueCreate(DUT_UART_DATA_QUEUE_DEPTH, sizeof(DutRxData));
    if (dut_uart_data_queue == 0)
    {
        printf("failed to create dut rx data queue\n");
    }

    memory_cmd_queue = xQueueCreate(SD_CARD_CMD_DATA_QUEUE_DEPTH, sizeof(CommandToSdCardQueueElement));
    if (memory_cmd_queue == 0)
    {
        printf("failed to create mem cmd queue\n");
    }

    memory_context.data_handle = dut_uart_data_queue;
    memory_context.command_handle = memory_cmd_queue;

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

    dut_context.rx_data_handle = dut_uart_data_queue;

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

    const auto btn_led_task_result = xTaskCreate(
        button_led_task,
        "LED",
        BUTTON_LED_STACK_SIZE,
        &btn_led_context,
        BUTTON_LED_TASK_PRIO,
        &btn_led_task_handle
    );
    if (pdPASS != btn_led_task_result)
    {
        printf("failed to craete task led btn, result=%d\n");
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

