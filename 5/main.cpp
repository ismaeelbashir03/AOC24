#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using InputType = std::pair<std::vector<std::pair<int, int>>, std::vector<std::vector<int> >>;

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

InputType readFile() {
    std::ifstream file("data.txt");
    std::vector<std::pair<int, int>> pairs;
    std::vector<std::vector<int>> rules;
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("|") != std::string::npos) {
            std::vector<std::string> tokens = split(line, '|');
            pairs.emplace_back(std::stoi(tokens[0]), std::stoi(tokens[1]));
        }
        else if (line.find(",") != std::string::npos) {
            std::vector<int> vec;
            std::vector<std::string> tokens = split(line, ',');
            for (const std::string& token: tokens) {
                vec.push_back(std::stoi(token));
            }
            rules.push_back(vec);
        }
    }
    return std::make_pair(pairs, rules);
}

int mid(std::vector<int>& rule) {
    int mid_i = rule.size() / 2;
    return rule[mid_i];
}

std::vector<int> topologicalSort(const std::vector<int>& rule, const std::vector<std::pair<int, int>>& pairs) {
    std::unordered_map<int, std::vector<int>> graph;
    std::unordered_map<int, int> inDegree;

    for (int page : rule) {
        graph[page] = {};
        inDegree[page] = 0;
    }

    for (const auto& pair : pairs) {
        int a = pair.first, b = pair.second;
        if (graph.count(a) && graph.count(b)) {
            graph[a].push_back(b);
            inDegree[b]++;
        }
    }

    std::queue<int> q;
    for (const auto& [node, degree] : inDegree) {
        if (degree == 0) q.push(node);
    }

    std::vector<int> sortedOrder;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        sortedOrder.push_back(current);

        for (int neighbor : graph[current]) {
            inDegree[neighbor]--;
            if (inDegree[neighbor] == 0) q.push(neighbor);
        }
    }

    return sortedOrder;
}

int main() {
    InputType inputPair = readFile();
    std::vector<std::pair<int, int>> pairs = inputPair.first;
    std::vector<std::vector<int>> rules = inputPair.second;
    
    // part1
    int total = 0;
    for (auto& rule: rules) {
        bool valid = true;
        std::unordered_map<int, int> position;
        for (int i = 0; i < rule.size(); i++) {
            position[rule[i]] = i;
        }
        for (auto& pair: pairs) {
            if (position.find(pair.first) == position.end() || position.find(pair.second) == position.end()) {
                continue;
            }
            if (position[pair.first] > position[pair.second]) {
                valid = false;
                break;
            }
        }
        if (valid) total += mid(rule);
    }
    std::cout << "Part1: " << total << std::endl;

    // Part2
    int total2 = 0;
    for (auto& rule: rules) {
        bool valid = true;
        std::unordered_map<int, int> position;
        for (int i = 0; i < rule.size(); i++) {
            position[rule[i]] = i;
        }
        for (auto& pair: pairs) {
            if (position.find(pair.first) == position.end() || position.find(pair.second) == position.end()) {
                continue;
            }
            if (position[pair.first] > position[pair.second]) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            rule = topologicalSort(rule, pairs);
            total2 += mid(rule);
        }
    }
    std::cout << "Part2: " << total2 << std::endl;
    return 0;
}