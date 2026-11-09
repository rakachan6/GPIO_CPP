#include "led.h"

#include <cassert>

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
        assert(interval_ms_ < 6000);
    }

    void start(uint32_t now)
    {
        start_time_ = now;
        is_running_ = true;
    }
    void stop()
    {
        is_running_ = false;
    }
    bool update(uint32_t now)
    {
        if (is_running_)
        {
            if ((now - start_time_) >= interval_ms_)
            {
                led_.toggle();
                start_time_ = now;
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    bool is_running() const
    {
        return is_running_;
    }
private:
    Led_type led_;
    uint32_t interval_ms_;
    uint32_t start_time_ = 0;
    bool is_running_ = false;
};

extern "C" void app_run(void)
{
    //Led<0> led_zero(GPIOC);
    //Led<GPIO_PIN_1 | GPIO_PIN_2> led_double(GPIOC);
    Led<GPIO_PIN_13> led_PC13(GPIOC, Active_level::low);
    Blinker<Led<GPIO_PIN_13>> blinker(led_PC13, 200);

    blinker.start(HAL_GetTick());
    while (true)
    {
        /*
        led_PC13.on();
        HAL_Delay(1000);
        led_PC13.off();
        HAL_Delay(1000);
        for (uint8_t i = 0; i < 6; i++)
        {
            led_PC13.toggle();
            HAL_Delay(500);
        }
        */
        blinker.update(HAL_GetTick());
    }
}