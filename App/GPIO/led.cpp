#include "led.h"
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
                    (pin_mask & (pin_mask - 1)) == 0);
public:
    Led(GPIO_TypeDef* port,
        Active_level level = Active_level::high) :
    port_(port), active_level_(level) {}
    void on()
    {
        HAL_GPIO_WritePin(
            port_, pin_mask,
            active_level_ == Active_level::high
                ? GPIO_PIN_SET
                : GPIO_PIN_RESET);
    }

    void off()
    {
        HAL_GPIO_WritePin(
            port_, pin_mask,
            active_level_ == Active_level::high
                ? GPIO_PIN_RESET
                : GPIO_PIN_SET);
    }

    void toggle()
    {
        HAL_GPIO_TogglePin(port_, pin_mask);
    }
private:
    GPIO_TypeDef* port_;
    const Active_level active_level_;
};

extern "C" void app_run(void)
{
    //Led<0> led_zero(GPIOC);
    //Led<GPIO_PIN_1 | GPIO_PIN_2> led_double(GPIOC);
    Led<GPIO_PIN_13> led_PC13(GPIOC, Active_level::low);
    led_PC13.off();
    while (1)
    {
        led_PC13.on();
        HAL_Delay(1000);
        led_PC13.off();
        HAL_Delay(1000);
        for (uint8_t i = 0; i < 6; i++)
        {
            led_PC13.toggle();
            HAL_Delay(500);
        }
    }
}