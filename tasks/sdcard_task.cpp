#include "FreeRTOS.h"
#include "task.h"

#include "f_util.h"
#include "ff.h"

#include <stdio.h>

#include "sdcard_task.h"
#include "sd_card.h"
#include "hw_config.h"

MemoryContext * _context{nullptr};

void sdcard_thread(void *context)
{
    _context = (MemoryContext *) context;

    // See FatFs - Generic FAT Filesystem Module, "Application Interface",
    // http://elm-chan.org/fsw/ff/00index_e.html
    sd_card_t *pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (FR_OK != fr) panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    FIL fil;
    const char* const filename = "filename.txt";
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    if (f_printf(&fil, "Hello, world!\n") < 0) {
        printf("f_printf failed\n");
    }
    fr = f_close(&fil);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    f_unmount(pSD->pcName);

    printf("starting sdcard task loop");
    while(1) 
    {
        // check commands queue

        // check data queue

        vTaskDelay(1000);
    }
}