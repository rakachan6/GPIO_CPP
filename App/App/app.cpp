#include "cstdint"
#include "cstring"
#include "iostream"
#include "app.h"
#include "led.h"
#include "uart.h"
#include "main.h"

extern UART_HandleTypeDef huart1;

extern uint8_t data[UART_DATA_SIZE];
extern bool is_received;

Led<GPIO_PIN_13> led(GPIOC, Active_level::low);
Blinker<Led<GPIO_PIN_13>> blinker(led, 500U);
Uart uart1(huart1);

char message[64];

void str_proc()
{
    if (!strncmp((char*)data, "1", 1))
    {
        blinker.start(HAL_GetTick());
        uart1.transmit_it((uint8_t*)"ON\r\n", 4U);
    }
    else if (!strncmp((char*)data, "0", 1))
    {
        blinker.stop();
        uart1.transmit_it((uint8_t*)"OFF\r\n", 4U);
    }
    else if (!strncmp((char*)data, "+", 1))
    {
        if (blinker.inc_interval(100U) == false)
            uart1.transmit_it((uint8_t*)"ERROR\r\n", 7U);
    }
    else if (!strncmp((char*)data, "-", 1))
    {
        if (blinker.dec_interval(100U) == false)
            uart1.transmit_it((uint8_t*)"ERROR\r\n", 7U);
    }
    else if (!strncmp((char*)data, "?", 1))
    {
        uint32_t interval_ms = blinker.get_interval_ms();
        bool status = blinker.is_running();
        uint8_t len;
        len = snprintf(message, sizeof(message),
                "LED: %s, interval: %lu ms\r\n",
                status? "ON" : "OFF", interval_ms);
        uart1.transmit_it((uint8_t*)message, len);
    }
    is_received = false;
    uart1.start_receive_to_idle_it(data, UART_DATA_SIZE);
}

void app_main_while()
{
    blinker.update(HAL_GetTick());

    if (is_received) // UART received command
    {
        str_proc();
    }
}

void app_init(void)
{
    if (uart1.start_receive_to_idle_it(data, UART_DATA_SIZE) != HAL_OK)
        Error_Handler();

    blinker.start(HAL_GetTick());
}