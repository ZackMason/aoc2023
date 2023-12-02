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

#include <immintrin.h>

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

struct _auto_deferer_t {
    std::function<void(void)> fn;

    template<typename Fn>
    _auto_deferer_t(Fn _fn) 
        : fn{_fn}
    {
    }

    template<typename Fn>
    _auto_deferer_t& operator=(Fn _fn) {
        fn = _fn;
        return *this;
    }

    ~_auto_deferer_t() {
        fn();
    }
};

#define VAR_CAT(a,b) a ## b
#define VAR_CAT_(a,b) VAR_CAT(a, b)
#define defer _auto_deferer_t VAR_CAT_(_res_auto_defer_, __LINE__) = [&]()

namespace utl {

    using string_pair = std::pair<std::string_view, std::string_view>;

    // returns [before, after]
    constexpr string_pair cut(std::string_view text, std::string_view delim) {
        auto pos = text.find_first_of(delim);
        if (pos != std::string_view::npos) {
            return {text.substr(0, pos), text.substr(pos+1)};
        } else {
            return {text, ""sv};
        }
    }

}
