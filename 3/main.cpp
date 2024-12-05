#include <cstdlib>
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

std::string readFile() {
    std::ifstream file("data.txt");
    std::string bytes((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
    return bytes;
}

int main() {
    std::string memory = readFile();
    
    std::regex mulRegex(R"(\bmul\((\d+),(\d+)\))"); // regex of mul(x,y)
    std::smatch match;
    std::string::const_iterator searchStart(memory.cbegin());
    int total = 0;

    while (std::regex_search(searchStart, memory.cend(), match, mulRegex)) {
        int x = std::stoi(match[1].str());
        int y = std::stoi(match[2].str());
        total += x * y;
        
        // move search up to end of last match
        searchStart = match.suffix().first;
    }

    std::cout << "Part1: " << total << std::endl;

    // Part2
    std::regex AllRegex(R"((\bdo\(\))|(\bdon't\(\))|(\bmul\((\d+),(\d+)\)))");
    searchStart = memory.cbegin();
    int total2 = 0;
    bool enabled = true;

    while (std::regex_search(searchStart, memory.cend(), match, AllRegex)) {
        if (match[1].matched) { // do()
            enabled = true;
        } else if (match[2].matched) { // don't()
            enabled = false;
        } else if (match[3].matched) { // mul(x,y)
            if (enabled) {
                int x = std::stoi(match[4].str());
                int y = std::stoi(match[5].str());
                total2 += x * y;
            }
        }
        searchStart = match.suffix().first;
    }
    std::cout << "Part2: " << total2 << std::endl;
    return 0;
}