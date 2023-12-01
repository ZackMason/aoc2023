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
