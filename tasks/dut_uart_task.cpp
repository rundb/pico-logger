#include "FreeRTOS.h"
#include "task.h"

#include <pico/stdlib.h>
#include <stdio.h>

#include "dut_uart_task.h"

#include <cstring>

// TODO: move to a dedicated configuration file
#define DUT_UART_TX (0)
#define DUT_UART_RX (1)
#define DUT_UART_INTERFACE (uart0)
#define DUT_UART_BAUDRATE (115200)

#define DUT_RX_BUFFER_SIZE (DUT_RX_DATA_MAX_DATA_SIZE)


DutUartContext * _dut_context{nullptr};

void dut_uart_init()
{
    gpio_set_function(DUT_UART_TX, GPIO_FUNC_UART);
    gpio_set_function(DUT_UART_RX, GPIO_FUNC_UART);
    gpio_set_pulls(DUT_UART_TX, 1, 0);
    gpio_set_pulls(DUT_UART_RX, 1, 0);
    uart_init(DUT_UART_INTERFACE, DUT_UART_BAUDRATE);
}

void dut_uart_task(void * context)
{
    uint8_t rx_buf[DUT_RX_BUFFER_SIZE + 1]{0};

    dut_uart_init();
    _dut_context = reinterpret_cast<DutUartContext *>(context);

    printf("task dut uart: start\n");
    uint32_t rx_len = 0;
    while (1)
    {
        rx_len = 0;
        while(uart_is_readable(DUT_UART_INTERFACE) && (rx_len < DUT_RX_BUFFER_SIZE)) {
            rx_buf[rx_len++] = uart_getc(DUT_UART_INTERFACE);
        }
        if (rx_len == 0)
        {
            vTaskDelay(pdMS_TO_TICKS(2));
        }
        else
        {
            DutRxData d;
            d.size = rx_len;
            memcpy(d.data, &rx_buf, rx_len);
            xQueueSend(_dut_context->rx_data_handle, reinterpret_cast<void*>(&d), 0);
        }
    }

}