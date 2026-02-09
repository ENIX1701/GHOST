#pragma once
#include <array>
#include <string>

namespace Obfuscation {
    constexpr char KEY = 0x44; // TODO: parametrize in CMake

    // please note: it's my first time ever using this kind of construct
    // it may or may not be written badly
    // it may or may not end up working
    // I really want to try it tho...
    template <size_t N>
    struct XorString {
        std::array<char, N> buffer;

        consteval XorString(const char (&str)[N]) {
            for (size_t i = 0; i < N; i++) {
                buffer[i] = str[i] ^ KEY;
            }
        }

        std::string decrypt() const {
            std::string result;
            result.reserve(N);

            for (size_t i = 0; i < N; i++) {
                if (buffer[i]) {    // null terminator check
                    result.push_back(buffer[i] ^ KEY);
                }
            }

            if (!result.empty() && result.back() == '\0') {
                result.pop_back();
            }

            return result;
        }
    };
}

// macros for ease of use :3
#define OBFL(str) (Obfuscation::XorString<sizeof(str)>(str).decrypt())
#define OBF(str) (Obfuscation::XorString<sizeof(str)>(str).decrypt().c_str())
