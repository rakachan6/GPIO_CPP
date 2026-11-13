#include <cstddef>
#include <cstdint>
#include "stm32f4xx_hal.h"

const uint16_t UART_DATA_SIZE = 256U;
uint8_t data[UART_DATA_SIZE];

class Uart {
public:
    explicit Uart(UART_HandleTypeDef& huart) noexcept : huart_(huart) {}

    Uart(const Uart&) = delete;
    Uart& operator=(const Uart&) = delete;
    ~Uart()
    {
        HAL_UART_DeInit(&huart_);
    }

    HAL_StatusTypeDef start_receive_to_idle_it(
        uint8_t* buffer, size_t capacity) const
    {
        if (buffer == nullptr || capacity == 0 || capacity > UINT16_MAX)
            return HAL_ERROR;

        return HAL_UARTEx_ReceiveToIdle_IT(
            &huart_, buffer, static_cast<uint16_t>(capacity));
    }

    HAL_StatusTypeDef transmit_blocking(
        uint8_t* data, size_t size, uint32_t timeout_ms) const
    {
        if (data == nullptr || size == 0 || size > UINT16_MAX)
            return HAL_ERROR;

        return HAL_UART_Transmit(
            &huart_, data, static_cast<uint16_t>(size), timeout_ms);
    }

    HAL_StatusTypeDef transmit_it(
        uint8_t* data, size_t size) const
    {
        if (data == nullptr || size == 0 || size > UINT16_MAX)
            return HAL_ERROR;

        return HAL_UART_Transmit_IT(
            &huart_, data, static_cast<uint16_t>(size));
    }

private:
    UART_HandleTypeDef& huart_;
};