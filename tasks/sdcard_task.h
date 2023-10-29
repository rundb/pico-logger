#pragma once

#include "FreeRTOS.h"
#include "queue.h"

#include <cstdint>

enum CommandToSdCard: std::uint8_t
{
    CREATE_LOG_RECORD,
    CLOSE_LOG_RECORD,
    UNMOUNT_SDCARD,
};

struct CommandToSdCardQueueElement
{
    CommandToSdCard command;
};

struct MemoryContext
{
    QueueHandle_t command_handle{nullptr};
    QueueHandle_t data_handle{nullptr};
};

void sdcard_thread(void *context);
