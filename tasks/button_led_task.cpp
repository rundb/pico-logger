#include "button_led_task.h"

#include <stdio.h>

#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "sdcard_task.h"

#define SD_CARD_ACTIVE_INPUT_PIN (16)

ButtonLedContext * _button_led_context{nullptr};

bool is_sd_card_state_pin_high{false};

bool was_close_log_command_sent{false};
bool was_unmount_command_sent{false};

static constexpr uint32_t close_log_command_tick = pdMS_TO_TICKS(5000);
static constexpr uint32_t unmount_command_tick = close_log_command_tick + pdMS_TO_TICKS(3000);

void button_led_task(void *context)
{
    _button_led_context = reinterpret_cast<ButtonLedContext *>(context);

    gpio_init(SD_CARD_ACTIVE_INPUT_PIN);
    gpio_set_dir(SD_CARD_ACTIVE_INPUT_PIN, GPIO_IN);

    const auto operation_start_tick = xTaskGetTickCount();

    printf("task led-btn: start\n");
    while(1)
    {
        // for test purposes this task would initially send commands based on time and not on the inputs
        const auto sd_card_state_pin = gpio_get(SD_CARD_ACTIVE_INPUT_PIN);
        if (sd_card_state_pin != is_sd_card_state_pin_high)
        {
            printf("sd card detect pin detected: %d->%d", is_sd_card_state_pin_high, sd_card_state_pin);
            is_sd_card_state_pin_high = sd_card_state_pin;
        }
        vTaskDelay(pdMS_TO_TICKS(10));

        const auto tick = xTaskGetTickCount();
        if (!was_close_log_command_sent && (tick - operation_start_tick) >= close_log_command_tick)
        {
            was_close_log_command_sent = true;
            CommandToSdCardQueueElement cmd;
            cmd.command = CommandToSdCard::CLOSE_LOG_RECORD;
            xQueueSend(_button_led_context->command_handle, reinterpret_cast<void *>(&cmd), 0);
        }

        if (!was_unmount_command_sent && (tick - operation_start_tick) >= unmount_command_tick)
        {
            was_unmount_command_sent = true;
            CommandToSdCardQueueElement cmd;
            cmd.command = CommandToSdCard::UNMOUNT_SDCARD;
            xQueueSend(_button_led_context->command_handle, reinterpret_cast<void *>(&cmd), 0);
        }

    }
}
