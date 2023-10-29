#include "FreeRTOS.h"
#include "task.h"

#include "f_util.h"
#include "ff.h"

#include <stdio.h>

#include "sdcard_task.h"
#include "sd_card.h"
#include "hw_config.h"

#include "dut_uart_task.h"

#define DUT_DATA_WAIT_TICKS (pdMS_TO_TICKS(10))

MemoryContext * _context{nullptr};

void sdcard_thread(void *context)
{
    _context = (MemoryContext *) context;

    DutRxData rx_data_buffer;
    CommandToSdCardQueueElement cmd_buffer;

    printf("task sdcard: start");

    // See FatFs - Generic FAT Filesystem Module, "Application Interface",
    // http://elm-chan.org/fsw/ff/00index_e.html
    sd_card_t *pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    
    if (FR_OK != fr) panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);

    printf("mount: ok\n");

    FIL fil;
    const char* const filename = "filename.txt";
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    printf("fopen: ok\n");
    if (f_printf(&fil, "Hello, world!\n") < 0) {
        printf("f_printf failed\n");
    }
    fr = f_close(&fil);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    printf("fclose: ok\n");
    f_unmount(pSD->pcName);
    printf("funmount: ok\n");

    printf("starting sdcard task loop\n");
    while(1) 
    {
        // check commands queue
        const auto cmd_rx_result = xQueueReceive(
            _context->command_handle,
            reinterpret_cast<void*>(&cmd_buffer),
            0
        );
        if (pdPASS != cmd_rx_result)
        {
            printf("sd card: received cmd %d", cmd_buffer.command);
        }

        // check data queue
        const auto dut_data_rx_result = xQueueReceive(
            _context->data_handle, 
            reinterpret_cast<void*>(&rx_data_buffer),
            DUT_DATA_WAIT_TICKS);
        if (pdPASS == dut_data_rx_result)
        {
            if (rx_data_buffer.size >= DUT_RX_DATA_MAX_DATA_SIZE)
            {
                
            }
            else 
            {
                rx_data_buffer.data[rx_data_buffer.size+1] = '\0';
                printf("%s", rx_data_buffer.data);
            }
        }
    }
}