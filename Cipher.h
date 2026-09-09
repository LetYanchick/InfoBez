#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace Cipher
{
    using Block = std::array<std::uint8_t, 8>;

    Block encryptA(std::uint32_t value);
    Block decryptA(const Block& block);

    Block encryptB(std::uint32_t value);
    Block decryptB(const Block& block);

    Block encryptDecoy(std::uint32_t value);
    Block decryptDecoy(const Block& block);

    // Restores an obfuscated string constant. It is intentionally
    // a generic operation rather than a function named after paths.
    std::string unpack(const unsigned char* data,
                       std::size_t size,
                       std::uint8_t key);
}
