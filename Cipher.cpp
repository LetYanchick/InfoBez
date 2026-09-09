#include "Cipher.h"

#include <algorithm>

namespace
{
    constexpr std::uint8_t KA[8] =
    {
        0x5A, 0x31, 0xC7, 0x92,
        0x14, 0xE3, 0x68, 0xAF
    };

    constexpr std::uint8_t KB[8] =
    {
        0xA5, 0x17, 0xD3, 0x6C,
        0x29, 0x81, 0xF0, 0x4B
    };

    constexpr std::uint8_t KD[8] =
    {
        0x3D, 0x6A, 0x91, 0x27,
        0xB4, 0x58, 0xC2, 0x0F
    };

    std::uint8_t rol8(std::uint8_t x, unsigned n)
    {
        return static_cast<std::uint8_t>(
            (x << n) | (x >> (8U - n))
        );
    }

    std::uint8_t ror8(std::uint8_t x, unsigned n)
    {
        return static_cast<std::uint8_t>(
            (x >> n) | (x << (8U - n))
        );
    }
}

Cipher::Block Cipher::encryptA(std::uint32_t value)
{
    Block b =
    {
        static_cast<std::uint8_t>(value),
        static_cast<std::uint8_t>(value >> 8),
        static_cast<std::uint8_t>(value >> 16),
        static_cast<std::uint8_t>(value >> 24),
        0xC3, 0x71, 0x2E, 0x94
    };

    for (std::size_t i = 0; i < b.size(); ++i)
        b[i] = rol8(static_cast<std::uint8_t>(b[i] ^ KA[i]), 3);

    const std::size_t p[8] = { 3, 6, 1, 7, 0, 5, 2, 4 };
    Block out{};

    for (std::size_t i = 0; i < 8; ++i)
        out[i] = b[p[i]];

    return out;
}

Cipher::Block Cipher::decryptA(const Block& block)
{
    const std::size_t p[8] = { 3, 6, 1, 7, 0, 5, 2, 4 };
    Block b{};

    for (std::size_t i = 0; i < 8; ++i)
        b[p[i]] = block[i];

    for (std::size_t i = 0; i < b.size(); ++i)
        b[i] = static_cast<std::uint8_t>(ror8(b[i], 3) ^ KA[i]);

    return b;
}

Cipher::Block Cipher::encryptB(std::uint32_t value)
{
    Block b =
    {
        static_cast<std::uint8_t>(value),
        static_cast<std::uint8_t>(value >> 8),
        static_cast<std::uint8_t>(value >> 16),
        static_cast<std::uint8_t>(value >> 24),
        0x42, 0x91, 0x2D, 0x73
    };

    for (std::size_t i = 0; i < 7; ++i)
    {
        b[i] = static_cast<std::uint8_t>(
            (static_cast<unsigned>(b[i]) + 17U * (i + 1U)) ^ KB[i]
        );
    }

    std::uint8_t check = 0;
    for (std::size_t i = 0; i < 7; ++i)
        check ^= b[i];

    b[7] = static_cast<std::uint8_t>(b[7] ^ check);

    std::swap(b[0], b[7]);
    std::swap(b[1], b[6]);
    std::swap(b[2], b[5]);
    std::swap(b[3], b[4]);

    return b;
}

Cipher::Block Cipher::decryptB(const Block& block)
{
    Block b = block;

    std::swap(b[0], b[7]);
    std::swap(b[1], b[6]);
    std::swap(b[2], b[5]);
    std::swap(b[3], b[4]);

    std::uint8_t check = 0;
    for (std::size_t i = 0; i < 7; ++i)
        check ^= b[i];

    b[7] = static_cast<std::uint8_t>(b[7] ^ check);

    for (std::size_t i = 0; i < 7; ++i)
    {
        b[i] = static_cast<std::uint8_t>(
            (static_cast<unsigned>(b[i]) ^ KB[i])
            - 17U * (i + 1U)
        );
    }

    return b;
}

Cipher::Block Cipher::encryptDecoy(std::uint32_t value)
{
    Block b =
    {
        static_cast<std::uint8_t>(value),
        static_cast<std::uint8_t>(value >> 8),
        0x19, 0xE4, 0x6B, 0x38, 0xA1, 0x00
    };

    std::uint8_t check = 0x5C;

    for (std::size_t i = 0; i < 7; ++i)
    {
        b[i] = static_cast<std::uint8_t>(
            (b[i] ^ KD[i]) + static_cast<unsigned>(i * 7U + 3U)
        );
        check ^= b[i];
    }

    b[7] = check;

    std::reverse(b.begin(), b.end());
    return b;
}

Cipher::Block Cipher::decryptDecoy(const Block& block)
{
    Block b = block;
    std::reverse(b.begin(), b.end());

    for (std::size_t i = 0; i < 7; ++i)
    {
        b[i] = static_cast<std::uint8_t>(
            (static_cast<unsigned>(b[i]) - (i * 7U + 3U)) ^ KD[i]
        );
    }

    return b;
}

std::string Cipher::unpack(const unsigned char* data,
                           std::size_t size,
                           std::uint8_t key)
{
    std::string result(size, '\0');

    for (std::size_t i = 0; i < size; ++i)
        result[i] = static_cast<char>(data[i] ^ key);

    return result;
}
