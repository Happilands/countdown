#include <iostream>

#include "src/countdown.h"

#include <ctime>

int main() {
    std::vector<int32_t> numbers;
    std::cout << "Enter starting numbers: \nStops when a non-positive number is entered\n";
    int32_t i;
    std::cin >> i;
    while (i > 0){
        numbers.push_back(i);
        std::cin >> i;
    }

    std::cout << "Enter result: \n";
    int32_t result;
    std::cin >> result;

    Countdown countdown{numbers, result};

    // START
    std::clock_t    start;
    start = std::clock();

    auto& solutions_including_duplicates = countdown.findSolutions();
    std::unordered_set<std::string> strings;
    countdown.removeDuplicatesAndConvertToStrings(solutions_including_duplicates, strings);

    // AAAAND TIME!
    double construct_time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);

    // I don't count the construct_time it takes to print the strings
    for(const auto & string : strings){
        std::cout << string << std::endl;
    }

    std::cout << "# solutions: " << strings.size() << std::endl;

    std::cout << "Time to construct strings: " << construct_time << " ms" << std::endl;

    double print_time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) - construct_time;

    std::cout << "Time to print strings: " << print_time << " ms" << std::endl;

    return 0;
}