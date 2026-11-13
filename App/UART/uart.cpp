#include "uart.h"

#include <cstddef>
#include <cstdint>
#include "stm32f4xx.h"


bool is_received = false;

extern "C" void RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UARTEx_ReceiveToIdle_IT(huart, data, sizeof(data));
    is_received = true;
}