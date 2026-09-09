#include "FileControl.h"

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <utime.h>

namespace FileControl
{
    bool exists(const std::string& path)
    {
        std::error_code ec;
        return std::filesystem::is_regular_file(path, ec);
    }

    bool createDirectory(const std::string& path)
    {
        std::error_code ec;
        std::filesystem::create_directories(path, ec);
        return !ec && std::filesystem::is_directory(path, ec);
    }

    bool read(const std::string& path, Cipher::Block& block)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
            return false;

        file.read(reinterpret_cast<char*>(block.data()),
                  static_cast<std::streamsize>(block.size()));

        return file.gcount() == static_cast<std::streamsize>(block.size());
    }

    bool write(const std::string& path, const Cipher::Block& block)
    {
        // Existing files are opened without truncation. Exactly 8 bytes
        // are overwritten, therefore file length remains unchanged.
        std::fstream file(
            path,
            std::ios::in | std::ios::out | std::ios::binary
        );

        if (!file)
        {
            std::ofstream create(path, std::ios::binary);
            if (!create)
                return false;

            create.write(
                reinterpret_cast<const char*>(block.data()),
                static_cast<std::streamsize>(block.size())
            );

            return create.good();
        }

        file.seekp(0);
        file.write(
            reinterpret_cast<const char*>(block.data()),
            static_cast<std::streamsize>(block.size())
        );

        return file.good();
    }

    bool getTimes(const std::string& path, Times& times)
    {
        struct stat st{};

        if (::stat(path.c_str(), &st) != 0)
            return false;

        times.access = st.st_atim;
        times.modify = st.st_mtim;

        return true;
    }

    bool restoreTimes(const std::string& path, const Times& times)
    {
        struct timespec ts[2];

        ts[0] = times.access;
        ts[1] = times.modify;

        return ::utimensat(
            AT_FDCWD,
            path.c_str(),
            ts,
            0
        ) == 0;
    }
}
