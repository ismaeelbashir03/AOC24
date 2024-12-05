#include <cstdlib>
#include <iostream>
#include <fstream>
#include <regex>
#include <utility>
#include <vector>

using InputType = std::vector<std::vector<char> >;

InputType readFile() {
    std::ifstream file("data.txt");
    InputType grid;
    std::string line;
    while (file >> line) {
        std::vector<char> row;
        for (auto c : line) {
            row.push_back(c);
        }
        grid.push_back(row);
    }
    return grid;
}

const std::vector<std::vector<std::pair<int, int>> > DIRECTIONS = { 
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}}, // right
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, // down
    {{0, 0}, {0, -1}, {0, -2}, {0, -3}}, // left
    {{0, 0}, {-1, 0}, {-2, 0}, {-3, 0}}, // up
    {{0, 0}, {1, 1}, {2, 2}, {3, 3}}, // down-right
    {{0, 0}, {-1, -1}, {-2, -2}, {-3, -3}}, // up-left
    {{0, 0}, {-1, 1}, {-2, 2}, {-3, 3}}, // down-left
    {{0, 0}, {1, -1}, {2, -2}, {3, -3}}, // up-right
};
const std::vector<char> XMAS = {'X', 'M', 'A', 'S'};

int countXMAS(int i, int j, InputType& grid) {
    int count = 0;
    for (auto dir: DIRECTIONS) {
        int nxt = 0;
        for (auto dx_dy: dir) {
            int dx = dx_dy.first;
            int dy = dx_dy.second;
            if (i+dx < grid.size() && j+dy < grid[0].size() && grid[i+dx][j+dy] == XMAS[nxt]) { // check next char
                nxt ++;
            } else { // break, we havent found match in this dir
                nxt = 0;
                break;
            }
        }
        if (nxt == XMAS.size()) {
            count++;
        }
    }
    return count;
}

const std::vector<std::pair<int, int>> DIAGONAL = {{0, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
const std::vector<std::vector<char> > MAS = { 
    {'A', 'M', 'S', 'M', 'S'},
    {'A', 'S', 'M', 'M', 'S'},
    {'A', 'M', 'S', 'S', 'M'},
    {'A', 'S', 'M', 'S', 'M'},
};

int countMAS(int i, int j, InputType& grid) {
    int count = 0;
    if (grid[i][j] != 'A') return count;

    for (auto option: MAS) {
        int nxt = 0;
        for (auto dx_dy: DIAGONAL) {
            int dx = dx_dy.first;
            int dy = dx_dy.second;
            if (i+dx < grid.size() && j+dy < grid[0].size() && grid[i+dx][j+dy] == option[nxt]) { // check next char
                nxt ++;
            } else { // break, we havent found match in this dir
                nxt = 0;
                break;
            }
        }
        if (nxt == DIAGONAL.size()) {
            count++;
        }
    }

    return count;
}

int main() {
    InputType grid = readFile();
    int total = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            total += countXMAS(i, j, grid);
        }
    }
    
    std::cout << "Part1: " << total << std::endl;

    // Part2
    int total2 = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            total2 += countMAS(i, j, grid);
        }
    }
    std::cout << "Part2: " << total2 << std::endl;
    return 0;
}