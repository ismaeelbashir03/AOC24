#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<int> > readFile() {
    std::ifstream file("data.txt");
    std::vector<std::vector<int> > reports;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> report;
        int num;
        while (iss >> num) {
            report.push_back(num);
        }
        reports.push_back(report);
    }
    return reports;
}

bool isSafe(std::vector<int>& report) {
    bool increasing = false;
    bool valid = true;

    for (int i = 1; i < report.size(); i++) {
        // 1 <= x <= 3
        if (std::abs(report[i] - report[i-1]) < 1 || std::abs(report[i] - report[i-1]) > 3) {
            valid = false;
            break;
        }

        // incereasing/decreasing check
        if (i == 1) {
            increasing = report[i] > report[i-1];
        }
        if ((increasing && report[i] < report[i-1]) || (!increasing && report[i] > report[i-1])) {
            valid = false;
            break;
        }
    }

    return valid;
}

int main() {
    std::vector<std::vector<int> > reports = readFile();
    
    // part 1
    int safe = 0;
    for (std::vector<int> report : reports) {
        if (isSafe(report)) {
            safe++;
        }
    }
    std::cout << "Part1: " << safe << std::endl;

    // part 2
    int safe2 = 0;
    for (std::vector<int> report : reports) {
        if (isSafe(report)) {
            safe2++;
            continue;
        }

        bool canRemove = false;
        for (int i = 0; i < report.size(); i++) {
            std::vector<int> newReport = report;
            newReport.erase(newReport.begin() + i); // remove index
            if (isSafe(newReport)) {
                canRemove = true;
                break;
            }
        }
        if (canRemove) safe2++;
    }
    std::cout << "Part2: " << safe2 << std::endl;
    return 0;
}