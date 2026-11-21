#include "cstdint"
#include "app.h"
#include "led.h"
#include "uart.h"
#include "cli.h"
#include "main.h"
#include "cstring"
#include "iostream"

extern UART_HandleTypeDef huart1;
extern uint8_t data[UART_DATA_SIZE];
extern bool is_received;

Led<GPIO_PIN_13, Active_level::low> led(GPIOC);
Blinker_pattern<Led<GPIO_PIN_13, Active_level::low>> steady_blinker(led, steady);
Blinker_pattern<Led<GPIO_PIN_13, Active_level::low>> flash_blinker(led, double_flash);
Blinker_pattern<Led<GPIO_PIN_13, Active_level::low>>* active_blinker = nullptr;

Uart uart1(huart1);
char message[128];

void set_mode(Mode mode, uint32_t now)
{
    Blinker_pattern<Led<GPIO_PIN_13, Active_level::low>>* next = nullptr;

    switch (mode)
    {
    case Mode::off:             break;
    case Mode::steady:          next = &steady_blinker; break;
    case Mode::flash_double:    next = &flash_blinker; break;
    case Mode::print_help:
        message[0] = 0;
        for (uint8_t i = 0; i < commands.size(); i++)
        {
            char row[25];
            sprintf(row, "%c - %s\n", commands[i].key, commands[i].description);
            strcat(message, row);
        }
        uart1.transmit_it(reinterpret_cast<uint8_t*>(message), strlen(message) - 1U);
    }

    if (next == active_blinker)
        return;

    if (active_blinker != nullptr)
        active_blinker->stop();

    active_blinker = next;

    if (active_blinker != nullptr)
        active_blinker->start(now);
}

void str_proc()
{
    is_received = false;

    if (const auto mode = decode_mode(data[0]))
        set_mode(mode.value(), HAL_GetTick());
}

void app_main_while()
{
    if (is_received) // UART received command
    {
        str_proc();

        if (uart1.start_receive_to_idle_it(data, UART_DATA_SIZE) != HAL_OK)
            Error_Handler();
    }

    if (active_blinker != nullptr)
        active_blinker->update(HAL_GetTick());
}

void app_init(void)
{
    set_mode(Mode::steady, HAL_GetTick());
    if (uart1.start_receive_to_idle_it(data, UART_DATA_SIZE) != HAL_OK)
        Error_Handler();
}