#pragma once

#include "Cipher.h"

#include <ctime>
#include <string>

namespace FileControl
{
    struct Times
    {
        timespec access{};
        timespec modify{};
    };

    bool exists(const std::string& path);
    bool read(const std::string& path, Cipher::Block& block);
    bool write(const std::string& path, const Cipher::Block& block);
    bool createDirectory(const std::string& path);

    bool restoreTimes(const std::string& path, const Times& times);
}
