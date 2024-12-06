#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using InputType = std::vector<std::vector<char>>;

enum Direction {
    LEFT,
    UP,
    RIGHT,
    DOWN
};

struct Coord {
    int x;
    int y;

    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }
};

struct CoordHash {
    size_t operator()(const Coord& coord) const {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y) << 1);
    }
};

struct State {
    Direction direction;
    Coord coord;

    bool operator==(const State& other) const {
        return direction == other.direction && coord == other.coord;
    }
};

struct StateHash {
    size_t operator()(const State& state) const {
        return std::hash<int>()(static_cast<int>(state.direction)) ^ (std::hash<int>()(state.coord.x) << 1) ^ (std::hash<int>()(state.coord.y) << 2);
    }
};

InputType readFile() {
    std::ifstream file("data.txt");
    std::vector<std::vector<char>> data;
    std::string line;
    while (std::getline(file, line)) {
        data.push_back(std::vector<char>(line.begin(), line.end()));
    }
    return data;
}

void forward(State& state) {
    switch (state.direction) {
        case Direction::DOWN: 
            state.coord.y += 1;
            break;
        case Direction::UP:
            state.coord.y -= 1;
            break;
        case Direction::LEFT:
            state.coord.x -= 1;
            break;
        case Direction::RIGHT:
            state.coord.x += 1;
            break;
    }
}

void turnRight(State& state) {
    state.direction = static_cast<Direction>((state.direction + 1) % 4);
}

bool isObstacle(const Coord& next, const InputType& grid) {
    if (next.x < 0 || next.y < 0 || next.x >= grid[0].size() || next.y >= grid.size()) {
        return false; // can walk out
    }
    return grid[next.y][next.x] == '#';
}

bool oob(const State& state, const InputType& grid) {
    return state.coord.x < 0 || state.coord.y < 0 || state.coord.x >= grid[0].size() || state.coord.y >= grid.size();
}

void move(State& state, const InputType& grid, std::unordered_set<Coord, CoordHash>& seen) {
    Coord next = state.coord;
    switch (state.direction) {
        case Direction::DOWN: next.y += 1; break;
        case Direction::UP: next.y -= 1; break;
        case Direction::LEFT: next.x -= 1; break;
        case Direction::RIGHT: next.x += 1; break;
    }

    if (isObstacle(next, grid)) {
        turnRight(state); 
    } else {
        state.coord = next; 
        if (!oob(state, grid)) seen.insert(state.coord); 
    }
}


void move2(State& state, const InputType& grid, std::unordered_set<State, StateHash>& seen) {
    Coord next = state.coord;
    switch (state.direction) {
        case Direction::DOWN: next.y += 1; break;
        case Direction::UP: next.y -= 1; break;
        case Direction::LEFT: next.x -= 1; break;
        case Direction::RIGHT: next.x += 1; break;
    }

    if (isObstacle(next, grid)) {
        turnRight(state); 
    } else {
        state.coord = next; 
    }
}

State getGuard(const InputType& grid) {
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            switch (grid[y][x]) {
                case '^': return {Direction::UP, {x, y}};
                case 'v': return {Direction::DOWN, {x, y}};
                case '>': return {Direction::RIGHT, {x, y}};
                case '<': return {Direction::LEFT, {x, y}};
            }
        }
    }
    return {};
}

bool checkLoop(const State& state, const std::unordered_set<State, StateHash>& seen) {
    return seen.find(state) != seen.end();
}

int main() {
    InputType grid = readFile();
    
    // part1
    State guard = getGuard(grid);
    std::unordered_set<Coord, CoordHash> seen;
    seen.insert(guard.coord); 

    while (!oob(guard, grid)) {
        move(guard, grid, seen);
    }

    // for (int y = 0; y < grid.size(); ++y) {
    //     for (int x = 0; x < grid[y].size(); ++x) {
    //         Coord coord{x, y};
    //         if (seen.find(coord) != seen.end()) {
    //             std::cout << "X";
    //         } else {
    //             std::cout << grid[y][x];
    //         }
    //     }
    //     std::cout << std::endl;
    // }

    std::cout << "Part1: " << seen.size() << std::endl;

    // part2
    int total2 = 0;
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x] == '#' || grid[y][x] == '^' || grid[y][x] == 'v' || grid[y][x] == '<' || grid[y][x] == '>') {
                continue;
            }

            char backup = grid[y][x];
            grid[y][x] = '#';

            guard = getGuard(grid);
            std::unordered_set<State, StateHash> visited;
            visited.insert(guard);

            bool isLoop = false;
            while (!oob(guard, grid)) {
                move2(guard, grid, visited);

                if (checkLoop(guard, visited)) {
                    isLoop = true;
                    break;
                }
                visited.insert(guard);
            }

            if (isLoop) {
                total2++;
            }

            grid[y][x] = backup;
        }
    }
    std::cout << "Part2: " << total2 << std::endl;

    return 0;
}