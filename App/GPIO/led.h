#ifndef GPIO_CPP_LED_H
#define GPIO_CPP_LED_H
#include "cassert"
#include "stm32f4xx.h"
#include "array"

enum class Active_level
{
    low,
    high
};

using Blink_pattern = std::array<uint32_t, 4>;

constexpr Blink_pattern double_flash{100, 100, 100, 700};
constexpr Blink_pattern steady{500, 500, 500, 500};

template<uint16_t pin_mask, Active_level level>
class Led
{
    static_assert((pin_mask != 0U) &&
                    ((pin_mask & (pin_mask - 1)) == 0),
                    "Led requires exactly one GPIO pin");
public:
    explicit Led(GPIO_TypeDef* port) noexcept : port_(port)
    {
        assert(port_ != nullptr);
    }
    void on() const noexcept
    {
        if constexpr (level == Active_level::high)
            HAL_GPIO_WritePin(port_, pin_mask, GPIO_PIN_SET);
        else
            HAL_GPIO_WritePin(port_, pin_mask, GPIO_PIN_RESET);
    }

    void off() const noexcept
    {
        if constexpr (level == Active_level::high)
            HAL_GPIO_WritePin(port_, pin_mask, GPIO_PIN_RESET);
        else
            HAL_GPIO_WritePin(port_, pin_mask, GPIO_PIN_SET);
    }

    void toggle() const noexcept
    {
        HAL_GPIO_TogglePin(port_, pin_mask);
    }
private:
    GPIO_TypeDef* port_;
};

template<typename Led_type>
class Blinker
{
public:
    explicit Blinker(
        Led_type& led,
        uint32_t interval_ms = 500U,
        uint16_t min_interval = 200U,
        uint16_t max_interval = 6000U):
        led_(led), interval_ms_(interval_ms),
        min_interval_(min_interval), max_interval_(max_interval)
    {
        assert(min_interval_ > 0U &&
        min_interval_ <= interval_ms_ &&
        interval_ms_ <= max_interval_);
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
        if (inc_ms > max_interval_ - interval_ms_)
            return false;

        interval_ms_ += inc_ms;
        return true;
    }
    bool dec_interval(uint8_t dec_ms)
    {
        if (dec_ms > interval_ms_ - min_interval_)
            return false;

        interval_ms_ -= dec_ms;
        return true;
    }
    uint32_t get_interval_ms() const noexcept
    {
        return interval_ms_;
    }
private:
    Led_type& led_;
    uint32_t interval_ms_;
    uint16_t min_interval_;
    uint16_t max_interval_;
    uint32_t last_switch_time_ = 0;
    bool is_running_ = false;
};

template<typename Led_type>
class Blinker_pattern
{
public:
    explicit  Blinker_pattern(
        Led_type& Led,
        const Blink_pattern& pattern) :
    led_(Led), pattern_(pattern) {}
    void start(uint32_t now)
    {
        led_.on();
        last_switch_time_ = now;
        i = 0;
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
        if ((now - last_switch_time_) >= pattern_[i])
        {
            led_.toggle();
            last_switch_time_ = now;
            i = (i + 1) % pattern_.size();
            return true;
        }
        else
            return false;
    }
    bool is_running() const noexcept
    {
        return is_running_;
    }
private:
    Led_type& led_;
    const Blink_pattern& pattern_;
    uint32_t last_switch_time_ = 0U;
    uint8_t i = 0U;
    bool is_running_ = false;
};

#ifdef __cplusplus

#endif

#endif //GPIO_CPP_LED_H
