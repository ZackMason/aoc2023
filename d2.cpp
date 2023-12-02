#include <utl.hpp>

struct game_t {
    i32 id;
    i32 r;
    i32 g;
    i32 b;

    i32 mr = 99999;
    i32 mg = 99999;
    i32 mb = 99999;

    b32 valid() {
        return r <= 12 && g <= 13 && b <= 14;
    }

    i32 power() {
        return r * g * b;
    }
};

game_t parse_game(std::string_view line) {
    game_t result = {};

    line = line.substr(line.find_first_of(' ')+1);

    const char* start = line.data();
    const char* end = line.data() + line.size();

    auto parse_result = std::from_chars(start, end, result.id);
    start = parse_result.ptr + 2;

    // fmt::print("Game: {}", result.id);

    std::string_view games{start, (size_t)(end - start)};
    auto game_pos = games.find_first_of(';')+1;
    std::string_view game = games.substr(0, game_pos);
    games = games.substr(game_pos);

    // fmt::print("{}", games);

    while(game_pos != std::string_view::npos) {
        if (game.empty()) break;
        // fmt::print("\t - {}\n", game);

        while(1) {
            i32 color_value = 0;
            b32 done = 0;
            u64 o = 0;
            if (game[0] == ' ') o = 1; // parsing hack
            auto r = std::from_chars(game.data() + o, game.data() + game.size(), color_value);
            if (color_value == 0) {
                break;
            }
            game = game.substr((size_t)(r.ptr - game.data())+1);

            auto color_pos = game.find_first_of(',');
            if (color_pos == std::string_view::npos) {
                color_pos = game.find_first_of(';');
                done = 1;
            }
            auto color = game.substr(0, color_pos);

            // fmt::print("\t\t - {}: {} - {}\n", color, color_value, game);
            if (color == "red"sv) {
                result.r = std::max(result.r, color_value);
                result.mr = std::min(result.mr, color_value);
                // result.r += color_value;
            }
            if (color == "green"sv) {
                result.g = std::max(result.g, color_value);
                result.mg = std::min(result.mg, color_value);
            }
            if (color == "blue"sv) {
                result.b = std::max(result.b, color_value);
                result.mb = std::min(result.mb, color_value);
            }

            if (done) break;

            game = game.substr(color.size()+2);
        }
        
        game_pos = games.find_first_of(';');
        if (game_pos == std::string_view::npos) {
            game_pos = games.size() - 1;
        }
        game_pos += 1;
        game = games.substr(0, game_pos);
        games = games.substr(game_pos);
    }
    // fmt::print("\n\n");

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {return -1;}

    std::ifstream f{argv[1]};
    i32 result = 0;
    i32 power = 0;
    for (std::string line; std::getline(f, line);) {
        auto game = parse_game(line);
        if (game.valid()) {
            result += game.id;
        }
        power += game.power();
    }

    fmt::print("Part 1: {}", result);
    fmt::print("Part 2: {}", power);
    
    return 0;
}