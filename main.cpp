#include <iostream>

#include "src/countdown.h"

#include <ctime>

int main() {
    /*std::vector<int32_t> numbers;
    std::cout << "Enter starting numbers: \nStops when a non-positive number is entered\n";
    int32_t i;
    std::cin >> i;
    while (i > 0){
        numbers.push_back(i);
        std::cin >> i;
    }

    std::cout << "Enter result: \n";
    int32_t result;
    std::cin >> result;*/

    Countdown countdown{{1, 3, 7, 10, 25, 50}, 765};

    // START
    std::clock_t    start;
    start = std::clock();

    auto& solutions_including_duplicates = countdown.findSolutions();

    double find_time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
    start = std::clock();

    std::unordered_set<std::string> strings;
    countdown.removeDuplicatesAndConvertToStrings(solutions_including_duplicates, strings);

    // AAAAND TIME!
    double construct_time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
    start = std::clock();

    // I don't count the construct_time it takes to print the strings
    for(const auto & string : strings){
        std::cout << string << std::endl;
    }

    std::cout << "# solutions: " << strings.size() << std::endl;
    std::cout << "Duplicates: " << solutions_including_duplicates.size() << std::endl;

    std::cout << "Time to find duplicates: " << find_time << " ms" << std::endl;
    std::cout << "Time to construct strings: " << construct_time << " ms" << std::endl;

    double print_time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);

    std::cout << "Time to print strings: " << print_time << " ms" << std::endl;


    std::cout << "Press any key twice to exit." << std::endl;
    std::cin.get();
    std::cin.get();

    return 0;
}