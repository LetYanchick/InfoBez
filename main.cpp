#include "Application.h"
#include "Cipher.h"
#include "FileControl.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

int main()
{
    // домашняя директория
    const char* h = std::getenv("HOME");
    if (h == nullptr)
        return 1;

    std::string root(h);
    // ключи для расшифровки пути
    static const unsigned char q1[] = {
        0x72, 0x73, 0x3E, 0x32, 0x33, 0x3B, 0x34, 0x3A, 0x72, 0x73, 0x3E, 0x3C, 0x3E, 0x35, 0x38, 0x72, 0x73, 0x2E, 0x24, 0x2E, 0x39, 0x3C, 0x29, 0x3C, 0x72, 0x30, 0x73, 0x39, 0x3C, 0x29
    };

    static const unsigned char q2[] = {
        0x88, 0x89, 0xCB, 0xC8, 0xC4, 0xC6, 0xCB, 0x88, 0xD4, 0xCF, 0xC6, 0xD5, 0xC2, 0x88, 0x89, 0xC6, 0xD7, 0xD7, 0xC3, 0xC6, 0xD3, 0xC6, 0x88, 0xDF, 0x89, 0xC3, 0xC6, 0xD3
    };

    static const unsigned char q3[] = {
        0x1E, 0x1F, 0x52, 0x50, 0x52, 0x59, 0x54, 0x1E, 0x1F, 0x55, 0x54, 0x42, 0x5A, 0x45, 0x5E, 0x41, 0x1C, 0x55, 0x50, 0x45, 0x50, 0x1E, 0x55, 0x54, 0x42, 0x5A, 0x45, 0x5E, 0x41, 0x1F, 0x55, 0x50, 0x45
    };
    // создание директорий
    std::string p0 = root + Cipher::unpack(q1, sizeof(q1), 0x5D);
    std::string p1 = root + Cipher::unpack(q2, sizeof(q2), 0xA7);
    std::string p2 = root + Cipher::unpack(q3, sizeof(q3), 0x31);

    // вырезаем имя каталога
    const std::string d0 = p0.substr(0, p0.find_last_of('/'));
    const std::string d1 = p1.substr(0, p1.find_last_of('/'));
    const std::string d2 = p2.substr(0, p2.find_last_of('/'));

    FileControl::createDirectory(d0);
    FileControl::createDirectory(d1);
    FileControl::createDirectory(d2);

    if (!FileControl::exists(p0) &&
        !FileControl::exists(p1) &&
        !FileControl::exists(p2)) // если ни одного файла не существует, значит это первый запуск
    {
        FileControl::write(p0, Cipher::encryptA(0));
        FileControl::write(p1, Cipher::encryptB(0));
        FileControl::write(p2, Cipher::encryptDecoy(17));
    }
    else
    {
        if (!FileControl::exists(p0))
            goto z17;

        if (!FileControl::exists(p1))
            goto z17;

        if (!FileControl::exists(p2))
            goto z17;
    }

    {
        FileControl::Times t0{}; // запоминаем время до внесения изменений
        FileControl::Times t1{};

        if (!FileControl::getTimes(p0, t0))
            goto z17;

        if (!FileControl::getTimes(p1, t1))
            goto z17;

        Cipher::Block a{};
        Cipher::Block b{};
        Cipher::Block d{};

        if (!FileControl::read(p0, a))
            goto z17;
        // это рандомные вычисления
        volatile std::uint32_t n0 = 0x13579BDFu;
        n0 ^= 0x2468ACE0u;
        n0 += 17u;

        if (!FileControl::read(p1, b))
            goto z17;
        // это рандомные вычисления
        volatile std::uint32_t n1 = n0 * 3u + 11u;
        n1 ^= 0xA55AA55Au;

        if (!FileControl::read(p2, d))
            goto z17;

    
        Cipher::Block va = Cipher::decryptA(a);
        // это рандомные вычисления
        volatile std::uint32_t n2 =
            static_cast<std::uint32_t>(va[0]) * 257u + va[1];
        (void)n2;

        Cipher::Block vb = Cipher::decryptB(b);
        // это рандомные вычисления
        volatile std::uint32_t n3 =
            static_cast<std::uint32_t>(vb[2]) * 131u + vb[3];
        (void)n3;

        Cipher::Block vd = Cipher::decryptDecoy(d);

        // проверка контрольной суммы фальшивки прям тут
        const std::uint8_t dk[7] =
            { 0x3D, 0x6A, 0x91, 0x27, 0xB4, 0x58, 0xC2 };

        std::uint8_t dc = 0x5C;
        for (std::size_t i = 0; i < 7; ++i)
        {
            const std::uint8_t x = static_cast<std::uint8_t>(
                (static_cast<unsigned>(vd[i]) ^ dk[i])
                + (i * 7U + 3U)
            );
            dc ^= x;
        }
        // проверка служебных байтов контрольных файлов
        if (va[4] != 0xC3 ||
            va[5] != 0x71 ||
            va[6] != 0x2E ||
            va[7] != 0x94)
            goto z17;

        if (vb[4] != 0x42 ||
            vb[5] != 0x91 ||
            vb[6] != 0x2D ||
            vb[7] != 0x73)
            goto z17;

        if (dc != vd[7])
            goto z17;
        // из байт в цифру
        const std::uint32_t v0 =
            static_cast<std::uint32_t>(va[0]) |
            (static_cast<std::uint32_t>(va[1]) << 8) |
            (static_cast<std::uint32_t>(va[2]) << 16) |
            (static_cast<std::uint32_t>(va[3]) << 24);

        const std::uint32_t v1 =
            static_cast<std::uint32_t>(vb[0]) |
            (static_cast<std::uint32_t>(vb[1]) << 8) |
            (static_cast<std::uint32_t>(vb[2]) << 16) |
            (static_cast<std::uint32_t>(vb[3]) << 24);
        // счетчики должны совпасть
        if (v0 != v1)
            goto z41;
        // и быть меньше 3
        if (v0 > 3u)
            goto z41;

        goto z52;

    z41:// это рандомные вычисления
        {
            volatile std::uint32_t r7 = 0x31415926u;
            r7 ^= 0x27182818u;
            r7 += 9u;
        }
        goto z63;

    z52:// это рандомные вычисления
        {
            volatile std::uint32_t r8 = 0xCAFEBABEu;
            r8 = (r8 << 3) ^ (r8 >> 2);
            r8 += 13u;
        }
        goto z74;

    z74:// запуск
        {
            
            Application app;
            app.run();
        }

        {   // увеличили счетчик
            const std::uint32_t next = v1 + 1u;
            // зашифровали их
            const auto ea = Cipher::encryptA(next);
            const auto eb = Cipher::encryptB(next);
            // записали
            if (!FileControl::write(p0, ea))
                goto z17;

            if (!FileControl::write(p1, eb))
                goto z17;
            // поставили время изменений какое было до
            FileControl::restoreTimes(p0, t0);
            FileControl::restoreTimes(p1, t1);
        }

        return 0;
    }

z63:// это рандомные вычисления
    {
        volatile std::uint32_t r9 = 0x10203040u;
        r9 ^= 0x55667788u;
        r9 += 1u;
    }

    goto z17;

z17: // период окончен или взлом
    std::cout << "Trial period has ended or control data is invalid.\n";
    return 0;
}
