#include <utl.hpp>
#include <execution>

struct game_t {
    u64 id;
    u64 r;
    u64 g;
    u64 b;

    b32 valid() {
        return r <= 12 && g <= 13 && b <= 14;
    }

    u64 power() {
        return r * g * b;
    }
};

game_t parse_game(std::string_view line) {
    game_t result = {};

    line = line.substr(5);

    const char* start = line.data();
    const char* end = line.data() + line.size();

    auto parse_result = std::from_chars(start, end, result.id);
    start = parse_result.ptr + 2; // move to first game number

    std::string_view games{start, (size_t)(end - start)};
    u64 game_pos = 0;

    while(game_pos < games.size()) {
        u64 color_value = 0;
        auto r = std::from_chars(games.data() + game_pos, games.data() + games.size(), color_value);
        
        game_pos += u64((r.ptr) - (games.data() + game_pos)) + 1;
        
        switch(games[game_pos]) {
            case 'r':
                result.r = std::max(result.r, color_value);
                break;
            case 'g':
                result.g = std::max(result.g, color_value);
                break;
            case 'b':
                result.b = std::max(result.b, color_value);
                break;
        }
        
        while(std::isspace(games[game_pos]) == false) [[unlikely]] {
            game_pos += 1;
            if (game_pos >= games.size()) break;
        }
        game_pos += 1;
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {return -1;}

    std::atomic<u64> result = 0;
    std::atomic<u64> power = 0;

    std::ifstream f{argv[1]};

    f.seekg(0, std::ios::end);
    std::string file(f.tellg(), 0);
    f.seekg(0, std::ios::beg);
    f.read(file.data(), file.size());

    std::vector<std::string_view> lines;
    lines.reserve(500'001);

    for (auto [line, file_view] = utl::cut(file, "\n"sv);
        line != ""sv;
        std::tie(line, file_view) = utl::cut(file_view, "\n"sv)
    ) {
        lines.push_back(line);
    }

    std::for_each(
        std::execution::par,
        lines.begin(),
        lines.end(),
        [&](auto&& line)
    {
        auto game = parse_game(line);
        if (game.valid()) {
            result += game.id;
        }
        power += game.power();
    });

    fmt::print("Part 1: {}\n", result);
    fmt::print("Part 2: {}\n", power);
    
    return 0;
}