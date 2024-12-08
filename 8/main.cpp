#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>

using InputType = std::vector<std::vector<char> >;

struct Coord {
    int x;
    int y;

    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }
};

struct CoordHash {
    std::size_t operator()(const Coord& c) const {
        return std::hash<int>()(c.x) ^ std::hash<int>()(c.y);
    }
};

InputType readFile() {
    std::ifstream file("data.txt");
    InputType data;
    std::string line;
    
    while (std::getline(file, line)) {
        std::vector<char> nums;
        for (char& c : line) {
            nums.push_back(c);
        }
        data.push_back(nums);
    }
    return data;
}

Coord getAntiNodeLoc(InputType grid, Coord node, Coord other) {
    Coord antiNodeLoc;
    
    // get distance between node and other
    int xDiff = other.x - node.x;
    int yDiff = other.y - node.y;

    // go opposite direction of other from node
    if (xDiff > 0) {
        antiNodeLoc.x = node.x - std::abs(xDiff);
    } else if (xDiff < 0) {
        antiNodeLoc.x = node.x + std::abs(xDiff);
    } else {
        antiNodeLoc.x = node.x;
    }

    if (yDiff > 0) {
        antiNodeLoc.y = node.y - std::abs(yDiff);
    } else if (yDiff < 0) {
        antiNodeLoc.y = node.y + std::abs(yDiff);
    } else {
        antiNodeLoc.y = node.y;
    }

    return antiNodeLoc;
}

void setAntiNodes(std::unordered_set<Coord, CoordHash>& antiNodes, InputType grid, int x, int y) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == grid[x][y] && (i != x || j != y)) {
                Coord antiNodeLoc = getAntiNodeLoc(grid, {x, y}, {i, j});
                if (antiNodeLoc.x < 0 || antiNodeLoc.x >= grid.size() || antiNodeLoc.y < 0 || antiNodeLoc.y >= grid[0].size()) {
                    continue;
                }
                antiNodes.insert(antiNodeLoc);
            }   
        }
    }
}

std::vector<Coord> getAntiNodeLocs(InputType grid, Coord node, Coord other) {
    // get all on same line in grid bounds
    std::vector<Coord> antiNodeLocs;
    int xDiff = other.x - node.x;
    int yDiff = other.y - node.y;

    // go same direction
    for (int i = node.x + xDiff, j = node.y + yDiff; i >= 0 && i < grid.size() && j >= 0 && j < grid[0].size(); i += xDiff, j += yDiff) {
        antiNodeLocs.push_back({i, j});
    }

    // go back direction
    for (int i = node.x - xDiff, j = node.y - yDiff; i >= 0 && i < grid.size() && j >= 0 && j < grid[0].size(); i -= xDiff, j -= yDiff) {
        antiNodeLocs.push_back({i, j});
    }

    return antiNodeLocs;
}

void setAntiNodes2(std::unordered_set<Coord, CoordHash>& antiNodes, InputType grid, int x, int y) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == grid[x][y] && (i != x || j != y)) {
                std::vector<Coord> antiNodeLocs = getAntiNodeLocs(grid, {x, y}, {i, j});
                for (auto& antiNodeLoc : antiNodeLocs) {
                    if (antiNodeLoc.x < 0 || antiNodeLoc.x >= grid.size() || antiNodeLoc.y < 0 || antiNodeLoc.y >= grid[0].size()) {
                        continue;
                    }
                    antiNodes.insert(antiNodeLoc);
                }
            }   
        }
    }
}


int main() {
    InputType grid = readFile();

    // part1
    std::unordered_set<Coord, CoordHash> antiNodes;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] != '.') {
                setAntiNodes(antiNodes, grid, i, j);
            }
        }
    }
    // print out antiNodes as # for debugging
    // for (int i = 0; i < grid.size(); i++) {
    //     for (int j = 0; j < grid[i].size(); j++) {
    //         if (antiNodes.find({i, j}) != antiNodes.end()) {
    //             std::cout << "#";
    //         } else {
    //             std::cout << grid[i][j];
    //         }
    //     }
    //     std::cout << "\n";
    // }
    std::cout << "Part 1: " << antiNodes.size() << "\n";

    // part2
    std::unordered_set<Coord, CoordHash> antiNodes2;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] != '.') {
                setAntiNodes2(antiNodes2, grid, i, j);
            }
        }
    }
    // print out antiNodes as # for debugging
    // for (int i = 0; i < grid.size(); i++) {
    //     for (int j = 0; j < grid[i].size(); j++) {
    //         if (antiNodes2.find({i, j}) != antiNodes2.end()) {
    //             std::cout << "#";
    //         } else {
    //             std::cout << grid[i][j];
    //         }
    //     }
    //     std::cout << "\n";
    // }
    std::cout << "Part 2: " << antiNodes2.size() << "\n";
    return 0;
}