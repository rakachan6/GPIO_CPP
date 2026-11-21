#include "cli.h"
#include <array>
#include <cstdint>
#include <optional>

std::optional<Mode> decode_mode(uint8_t byte)
{
    for (uint8_t i = 0U; i < commands.size(); i++)
    {
        if (byte == commands[i].key)
        {
            return commands[i].mode;
        }
    }
    return std::nullopt;
}

constexpr bool is_unique_keys()
{
    for (size_t i = 0U; i < commands.size(); i++)
    {
        for (size_t j = i + 1U; j < commands.size(); j++)
        {
            if (commands[i].key == commands[j].key)
                return false;
        }
    }
    return true;
}
static_assert(is_unique_keys(), "Duplicate UART command");