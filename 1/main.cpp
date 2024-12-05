#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

std::pair<std::vector<int>, std::vector<int> > readFile() {
    std::ifstream file("data.txt");
    int first, second;
    std::vector<int> firsts;
    std::vector<int> seconds;
    while (file >> first >> second) {
        firsts.push_back(first);
        seconds.push_back(second);
    }
    return std::make_pair(firsts, seconds);
}

int main() {
    std::pair<std::vector<int>, std::vector<int> > data = readFile();
    
    // part 1
    std::sort(data.first.begin(), data.first.end());
    std::sort(data.second.begin(), data.second.end());
    int diff = 0;
    for (int i = 0; i < data.first.size(); i++) {
        if (data.first[i] > data.second[i]) {
            diff += std::abs(data.first[i] - data.second[i]);
        } else {
            diff += std::abs(data.second[i] - data.first[i]);
        }  
    }
    std::cout << "Part1: " << diff << std::endl;

    // part 2
    int score = 0;
    std::unordered_map<int, int> count; // item: count
    for (int num: data.second) {
        count[num] += 1;
    }
    for (int num: data.first) {
        score += num * count[num];
    }
    std::cout << "Part2: " << score << std::endl; 

    return 0;
}