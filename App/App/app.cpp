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

Led<GPIO_PIN_13, Active_level::low> led(GPIOC);
//Blinker<Led<GPIO_PIN_13, Active_level::low>> blinker(led, 500U);
Blinker_pattern<Led<GPIO_PIN_13, Active_level::low>> blinker(led, double_flash);
Uart uart1(huart1);

constexpr char ERROR_MSG[] = "ERROR\r\n";
char message[64];

void str_proc()
{
    if (data[0] == '1')
        blinker.start(HAL_GetTick());
    else if (data[0] == '0')
        blinker.stop();
    else if (data[0] == '+')
    {
        /*
        if (blinker.inc_interval(100U) == false)
            uart1.transmit_it(
                reinterpret_cast<const uint8_t*>(ERROR_MSG),
                sizeof(ERROR_MSG) - 1);
        */
    }
    else if (data[0] == '-')
    {
        /*
        if (blinker.dec_interval(100U) == false)
            uart1.transmit_it(
                reinterpret_cast<const uint8_t*>(ERROR_MSG),
                sizeof(ERROR_MSG) - 1);
        */
    }
    else if (data[0] == '?')
    {
        /*
        uint32_t interval_ms = blinker.get_interval_ms();
        bool status = blinker.is_running();
        uint8_t len;
        len = snprintf(message, sizeof(message),
                "LED: %s, interval: %lu ms\r\n",
                status? "ON" : "OFF", interval_ms
                );
        uart1.transmit_it((uint8_t*)message, len);
        */
    }
    is_received = false;

    if (uart1.start_receive_to_idle_it(data, UART_DATA_SIZE) != HAL_OK)
        Error_Handler();
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