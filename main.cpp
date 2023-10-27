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

MemoryContext memory_context;
static TaskHandle_t sdcard_taskhandle;

int main() {
    // stdio_init_all();
    stdio_uart_init_full(uart1, 115200, 20, 21);
    time_init();

    puts("Hello, world!");

    // // See FatFs - Generic FAT Filesystem Module, "Application Interface",
    // // http://elm-chan.org/fsw/ff/00index_e.html
    // sd_card_t *pSD = sd_get_by_num(0);
    // FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    // if (FR_OK != fr) panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    // FIL fil;
    // const char* const filename = "filename.txt";
    // fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    // if (FR_OK != fr && FR_EXIST != fr)
    //     panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    // if (f_printf(&fil, "Hello, world!\n") < 0) {
    //     printf("f_printf failed\n");
    // }
    // fr = f_close(&fil);
    // if (FR_OK != fr) {
    //     printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    // }
    // f_unmount(pSD->pcName);

    const auto result = xTaskCreate(sdcard_thread, "SDC", configMINIMAL_STACK_SIZE, &memory_context, SDCARD_TASK_PRIO, &sdcard_taskhandle);

    vTaskStartScheduler();
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

