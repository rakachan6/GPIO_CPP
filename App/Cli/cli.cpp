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