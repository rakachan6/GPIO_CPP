#ifndef GPIO_CPP_LED_H
#define GPIO_CPP_LED_H
#include "cassert"
#include "stm32f4xx.h"

enum class Active_level
{
    low,
    high
};

template<uint16_t pin_mask>
class Led
{
    static_assert((pin_mask != 0U) &&
                    ((pin_mask & (pin_mask - 1)) == 0),
                    "Led requires exactly one GPIO pin");
public:
    explicit Led(GPIO_TypeDef* port,
        Active_level level = Active_level::high) noexcept :
    port_(port), active_level_(level)
    {
        assert(port_ != nullptr);
    }
    void on() const noexcept
    {
        HAL_GPIO_WritePin(
            port_, pin_mask,
            active_level_ == Active_level::high
                ? GPIO_PIN_SET
                : GPIO_PIN_RESET);
    }

    void off() const noexcept
    {
        HAL_GPIO_WritePin(
            port_, pin_mask,
            active_level_ == Active_level::high
                ? GPIO_PIN_RESET
                : GPIO_PIN_SET);
    }

    void toggle() const noexcept
    {
        HAL_GPIO_TogglePin(port_, pin_mask);
    }
private:
    GPIO_TypeDef* port_;
    const Active_level active_level_;
};

template<typename Led_type>
class Blinker
{
public:
    explicit Blinker(
        Led_type& led,
        uint32_t interval_ms = 500) :
        led_(led), interval_ms_(interval_ms)
    {
        assert((interval_ms_ > 0U) && (interval_ms_ <= 60000U));
    }

    void start(uint32_t now)
    {
        led_.off();
        last_switch_time_ = now;
        is_running_ = true;
    }
    void stop()
    {
        led_.off();
        is_running_ = false;
    }
    bool update(uint32_t now)
    {
        if (!is_running_)
            return false;

        if ((now - last_switch_time_) >= interval_ms_)
        {
            led_.toggle();
            last_switch_time_ = now;
            return true;
        }
        else
            return false;
    }
    bool is_running() const
    {
        return is_running_;
    }
    bool inc_interval(uint8_t inc_ms)
    {
        if (interval_ms_ >= 5900U)
            return false;
        interval_ms_ += inc_ms;
        return true;
    }
    bool dec_interval(uint8_t dec_ms)
    {
        if (interval_ms_ <= 200U)
            return false;
        interval_ms_ -= dec_ms;
        return true;
    }
    uint32_t get_interval_ms()
    {
        return interval_ms_;
    }
private:
    Led_type& led_;
    uint32_t interval_ms_;
    uint32_t last_switch_time_ = 0;
    bool is_running_ = false;
};

#ifdef __cplusplus

#endif

#endif //GPIO_CPP_LED_H
