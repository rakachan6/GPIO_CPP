#include <cstddef>
#include <cstdint>
#include "stm32f4xx_hal.h"

class Uart {
public:
    explicit Uart(UART_HandleTypeDef& huart) noexcept : huart_(huart) {}

    Uart(const Uart&) = delete;
    Uart& operator=(const Uart&) = delete;
    ~Uart()
    {
        HAL_UART_DeInit(&huart_);
    }

    HAL_StatusTypeDef transmit_blocking(
        std::uint8_t* data, std::size_t size, std::uint32_t timeout_ms) const
    {
        if (data == nullptr || size == 0 || size > UINT16_MAX)
            return HAL_ERROR;

        return HAL_UART_Transmit(
            &huart_, data, static_cast<std::uint16_t>(size), timeout_ms);
    }

    HAL_StatusTypeDef start_transmit_it(
        std::uint8_t* data, std::size_t size) const
    {
        if (data == nullptr || size == 0 || size > UINT16_MAX)
            return HAL_ERROR;

        return HAL_UART_Transmit_IT(
            &huart_, data, static_cast<std::uint16_t>(size));
    }

private:
    UART_HandleTypeDef& huart_;
};