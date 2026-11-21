#ifndef GPIO_CPP_CLI_H
#define GPIO_CPP_CLI_H
#include <array>
#include <optional>
#include <cstdint>
#include "app.h"

struct Command
{
    char key;
    Mode mode;
    const char* description;
};

constexpr std::array<Command, 4> commands{{
    {'0', Mode::off,            "Led off"},
    {'1', Mode::steady,         "Steady blink"},
    {'2', Mode::flash_double,   "Double flash"},
    {'h', Mode::print_help,     "Print help message"}
}};

std::optional<Mode> decode_mode(uint8_t byte);

#endif //GPIO_CPP_CLI_H
