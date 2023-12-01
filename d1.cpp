#include <cstdint>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <span>
#include <fstream>
#include <chrono>
#include <functional>
#include <ranges>
#include <charconv>
#include <string>
#include <string_view>

#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/format.h>

using namespace std::string_view_literals;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using b8 = u8;
using b16 = u16;
using b32 = u32;
using b64 = u64;
using umm = size_t;

using f32 = float;
using f64 = double;

constexpr i32 INVALID = -1;

i32 isdigit(char c) {
    const i32 result = c - '0';
    return 0 <= result && result <= 9 ? result : INVALID;
}

i32 read(std::string_view number, i32 value, i32 index, const char* str, u64 size, i32 dir = 1) {
    i32 c = (i32)number.size();
    i32 r = dir > 1 ? index : index - c + 1;
    if (r < 0 || r >= size) {
        return INVALID;
    }
    
    for (i32 i = 0; i < c;) {
        if (number[i++] != str[r++]) {
            return INVALID;
        }
    }

    return value;
}

i32 read_word(i32 index, const char* str, u64 size, i32 dir = 1) {
    i32 result = INVALID;

    #define map(number, _, value) result = read(#number, value, index, str, size, dir); if (result >= 0) return result;

    map(zero,  ->, 0);
    map(one,   ->, 1);
    map(two,   ->, 2);
    map(three, ->, 3);
    map(four,  ->, 4);
    map(five,  ->, 5);
    map(six,   ->, 6);
    map(seven, ->, 7);
    map(eight, ->, 8);
    map(nine,  ->, 9);

    #undef map

    return INVALID;
}

int main(int argc, char* argv[]) {
    if (argc != 2) { return INVALID; }
    {
        std::ifstream f{argv[1]};
        i32 result = 0;
        
        for (std::string line; std::getline(f, line);) {
            i32 x = 0;
            for (auto c: line) {
                auto d = isdigit(c);
                if (d > INVALID) {
                    x = d * 10;
                    break;
                }
            }
            for (auto c: std::ranges::reverse_view{line}) {
                auto d = isdigit(c);
                if (d > INVALID) {
                    x += d;
                    break;
                }
            }
            result += x;
        }

        fmt::print("P1: {}\n", result);
    }
    {
        std::ifstream f{argv[1]};
        i32 result = 0;

        for (std::string line; std::getline(f, line);) {
            i32 x = 0;
            i32 ls = (i32)line.size();
            for(i32 i = 0; i < ls; i++) {
                auto d = isdigit(line[i]);
                if (d > INVALID) {
                    x = d * 10;
                    break;
                }
                d = read_word(i, line.data(), line.size());
                if (d > INVALID) {
                    x = d * 10;
                    break;
                }
            }
            for(i32 i = ls - 1; i >= 0; i--) {
                auto d = isdigit(line[i]);
                if (d > INVALID) {
                    x += d;
                    break;
                }
                d = read_word(i, line.data(), line.size(), -1);
                if (d > INVALID) {
                    x += d;
                    break;
                }
            }
            result += x;
        }
        fmt::print("P2: {}", result);
    }

    return 0;
}