#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using InputType = std::vector<std::vector<long long> >; // big ass numbers for this q

long long getResult(const std::vector<long long>& nums, const std::vector<char>& ops) {
    long long result = nums[0];
    for (size_t i = 0; i < ops.size(); ++i) {
        if (ops[i] == '+') {
            result += nums[i+1];
        } else {
            result *= nums[i+1];
        }
    }
    return result;
}


bool canGet(long long target, const std::vector<long long>& nums) {
    if (nums.size() == 1) return nums[0] == target;

    size_t numOps = nums.size() - 1;

    // bit mask backtracking
    for (int mask = 0; mask < (1 << numOps); ++mask) { // each bit repr each combination
        std::vector<char> ops(numOps, '+');
        for (size_t i = 0; i < numOps; ++i) {
            if (mask & (1 << i)) ops[i] = '*'; // use mul for each 1 bit otherwise use add
        }
        if (getResult(nums, ops) == target) {
            return true;
        }
    }
    return false;
}

long long getResult2(const std::vector<long long>& nums, const std::vector<char>& ops) {
    long long result = nums[0];
    for (size_t i = 0; i < ops.size(); ++i) {
        if (ops[i] == '+') {
            result += nums[i + 1];
        } else if (ops[i] == '*') {
            result *= nums[i + 1];
        } else if (ops[i] == '|') { // Concatenation operator
            std::string left = std::to_string(result);
            std::string right = std::to_string(nums[i + 1]);
            result = std::stoll(left + right);
        }
    }
    return result;
}

bool canGet2(long long target, const std::vector<long long>& nums) {
    if (nums.size() == 1) return nums[0] == target;

    size_t numOps = nums.size() - 1;

    // change to base 3 for 3 ops?
    long long totalCombs = pow(3, numOps); // 3 operators - + * || -> 3^numOps - 3 decisions for each branch
    for (long long mask = 0; mask < totalCombs; ++mask) {
        std::vector<char> ops(numOps);
        long long tempMask = mask;
        for (size_t i = 0; i < numOps; ++i) {
            int opCode = tempMask % 3; // 0 = + 1 = * 2 = || -> get LSB
            if (opCode == 0) ops[i] = '+';
            else if (opCode == 1) ops[i] = '*';
            else if (opCode == 2) ops[i] = '|';
            tempMask /= 3; // shift to get next bit
        }
        if (getResult2(nums, ops) == target) {
            return true;
        }
    }
    return false;
}

InputType readFile() {
    std::ifstream file("data.txt");
    InputType data;
    std::string line;
    
    while (std::getline(file, line)) {
        size_t colon = line.find(':');
        
        long long target = std::stoll(line.substr(0, colon)); 
        std::string rest = line.substr(colon + 1);
        
        std::vector<long long> nums;
        nums.push_back(target);
        
        std::stringstream ss(rest);
        long long num;
        while (ss >> num) {
            nums.push_back(num);
        }
        
        data.push_back(nums);
    }
    
    return data;
}


int main() {
    InputType data = readFile();

    // part1
    long long total = 0;
    for (auto& line : data) {
        long long target = line[0];
        std::vector<long long> nums(line.begin() + 1, line.end());
        if (canGet(target, nums)) {
            total += target;
        }
    }
    std::cout << "Part 1: " << total << "\n";

    // part2
    long long total2 = 0;
    for (auto& line : data) {
        long long target = line[0];
        line.erase(line.begin());
        if (canGet2(target, line)) {
            total2 += target;
        }
    }
    std::cout << "Part 2: " << total2 << "\n";
    return 0;
}