#include "cstdint"
#include "cstring"
#include "optional"
#include "app.h"
#include "led.h"
#include "uart.h"
#include "main.h"

extern UART_HandleTypeDef huart1;
extern uint8_t data[UART_DATA_SIZE];
extern bool is_received;

enum class Mode {off, steady, flash_double};

constexpr char ERROR_MSG[] = "ERROR\r\n";
char message[64];

Led<GPIO_PIN_13, Active_level::low> led(GPIOC);
Blinker_pattern<Led<GPIO_PIN_13, Active_level::low>> steady_blinker(led, steady);
Blinker_pattern<Led<GPIO_PIN_13, Active_level::low>> flash_blinker(led, double_flash);
Blinker_pattern<Led<GPIO_PIN_13, Active_level::low>>* active_blinker = nullptr;
Uart uart1(huart1);

std::optional<Mode> decode_mode(uint8_t byte)
{
    switch (byte)
    {
    case '0': return Mode::off;
    case '1': return Mode::steady;
    case '2': return Mode::flash_double;
    default:  return std::nullopt;
    }
}

void set_mode(Mode mode, uint32_t now)
{
    Blinker_pattern<Led<GPIO_PIN_13, Active_level::low>>* next = nullptr;

    switch (mode)
    {
    case Mode::off:             break;
    case Mode::steady:          next = &steady_blinker; break;
    case Mode::flash_double:    next = &flash_blinker; break;
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