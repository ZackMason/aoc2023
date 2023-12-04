#include <utl.hpp>


using grid_t = std::vector<std::string_view>;

grid_t grid;

bool is_symbol(char c) {
    return !std::isdigit(c) && c != '.' && c != '\n';
}

bool safe_index(i64 line, i64 read) {
    if (line < 0 || read < 0) return false;
    return (u64)line < grid.size() && (u64)read < grid[line].size();
}

i64 gear_ratio(size_t line, size_t read) {
    // fmt::print("Gear\n");

    i64 ratio = 0;
    i32 count = 0;
    for (i64 y = -1; y < 2; y++) {
        for (i64 x = -1; x < 2; x++) {
            i64 i = (i64)line + y;
            i64 j = (i64)read + x;
            if (x == 0 && y == 0) continue;

            if (safe_index(i,j)) {
                if (std::isdigit(grid[i][j])) {
                    i64 number_start = j;
                    i64 number_end = j;
                    while(std::isdigit(grid[i][number_start])) {
                        number_start -= 1;
                        if (number_start == 0) break;
                    }
                    if (number_start)
                        number_start += 1;
                    if (grid[i][number_start] == '.')
                        number_start += 1;
                    while(std::isdigit(grid[i][number_end])) {
                        number_end += 1;
                        if (number_end >= (i64)grid[i].size()) break;
                    }
                    if (number_end != (i64)grid[i].size())
                    number_end -= 1;
                    i64 number = 0;
                    std::string_view ns = grid[i].substr(number_start, number_end-number_start+1);
                    // fmt::print("Gear Number: {}\n", ns);
                    std::from_chars(ns.data(), ns.data() + ns.size(), number);
    
                    if (count == 0) {
                        ratio = number;
                    } else if (count == 1) {
                        ratio *= number;
                    }
                    x += number_end - j;
                    count += 1;
                }
            }
        }
    }

    return count != 1 ? ratio : 0;
}

i64 part_value(size_t line, size_t read, std::string_view number) {
    // fmt::print("Part: {}\n", number);
    i64 result = 0;
    std::from_chars(number.data(), number.data() + number.size(), result);

    for (i64 x = -1; x < (i64)number.size() + 1; x++) {
        for (i64 y = -1; y < 2; y++) {
            i64 i = (i64)line + y;
            i64 j = (i64)read + x;
            if (safe_index(i, j)) {
                if (is_symbol(grid[i][j])) {
                    // fmt::print("Part: {} has symbol {}\n", number, grid[i][j]);
                    return result;
                }
            }
        }
    }

    return 0;
}

void part_one();
void part_two();

int main(int argc, char* argv[]) {
    if (argc != 2) return -1;

    std::ifstream f{argv[1]};

    f.seekg(0, std::ios::end);
    std::string file(f.tellg(), 0);
    f.seekg(0, std::ios::beg);
    f.read(file.data(), file.size());

    
    for (auto [line, file_view] = utl::cut(file, "\n"sv);
        line != ""sv;
        std::tie(line, file_view) = utl::cut(file_view, "\n"sv)
    ) {
        grid.push_back(line);
    }

    part_one();
    part_two();
}

void part_one() {
    size_t line = 0;
    size_t read = 0;
    i64 result = 0;

    while(line < grid.size()) {
        while(read < grid[line].size()) {
            while(read < grid[line].size() && grid[line][read] == '.') {
                read++;
            }
            
            auto number_start = read;
            while(std::isdigit(grid[line][read])) {
                read++;
            }
            auto number_size = read - number_start;
            
            if (number_size != 0) {
                result += part_value(line, number_start, grid[line].substr(number_start, number_size));
            }

            read++;
        }
        read = 0;
        line++;
    }

    fmt::print("Part 1: {}\n", result);
}

void part_two() {
    size_t line = 0;
    size_t read = 0;
    i64 result = 0;

    while(line < grid.size()) {
        while(read < grid[line].size()) {
            while(read < grid[line].size() && (grid[line][read] == '.' || std::isdigit(grid[line][read]))) {
                read++;
            }
                        
            if (read < grid[line].size() && grid[line][read] == '*') {
                auto ratio = gear_ratio(line, read);
                result += ratio;
            }
            read++;
        }
        read = 0;
        line++;
    }

    fmt::print("Part 2: {}\n", result);
}