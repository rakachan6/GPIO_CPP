#include "led.h"
#include "stm32f4xx.h"

enum class Active_level
{
    low,
    high
};
class Led
{
public:
    Led(GPIO_TypeDef* port,
        uint16_t pin,
        Active_level level = Active_level::high) :
    port_(port), pin_(pin), level_(level) {}
    void on()
    {
        HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_SET);
        level_ = Active_level::high;
    }
    void off()
    {
        HAL_GPIO_WritePin(port_, pin_, GPIO_PIN_RESET);
        level_ = Active_level::low;
    }
    void toggle()
    {
        HAL_GPIO_TogglePin(port_, pin_);
        if (HAL_GPIO_ReadPin(port_, pin_) == GPIO_PIN_RESET)
            level_ = Active_level::low;
        else
            level_ = Active_level::high;
    }
private:
    GPIO_TypeDef* port_;
    uint16_t pin_;
    Active_level level_;
};

extern "C" void app_run(void)
{
    Led led_PC13(GPIOC, GPIO_PIN_13);
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