#include "uart.h"

#include <cstddef>
#include <cstdint>
#include "stm32f4xx.h"

uint8_t data[UART_DATA_SIZE];
bool is_received = false;

extern "C" void RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UARTEx_ReceiveToIdle_IT(huart, data, UART_DATA_SIZE);
    is_received = true;
}

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

    is_received = true;
}