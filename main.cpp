#include <iostream>

#include "src/countdown.h"

#include <ctime>
#include <chrono>

int findAllSolutions(){
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

    Countdown countdown{{1,3,7,10,25,50}, 765};

    // START
    auto start = std::chrono::high_resolution_clock::now();

    auto& solutions_including_duplicates = countdown.findSolutions();

    auto now = std::chrono::high_resolution_clock::now();
    long long find_time = std::chrono::duration_cast<std::chrono::microseconds>(
            now - start).count();
    start = now;

    std::unordered_set<std::string> strings;
    countdown.removeDuplicatesAndConvertToStrings(solutions_including_duplicates, strings);

    now = std::chrono::high_resolution_clock::now();
    long long construct_time = std::chrono::duration_cast<std::chrono::microseconds>(
            now - start).count();
    start = now;

    // I don't count the construct_time it takes to print the strings
    for(const auto & string : strings){
        std::cout << string << std::endl;
    }

    now = std::chrono::high_resolution_clock::now();
    long long print_time = std::chrono::duration_cast<std::chrono::microseconds>(
            now - start).count();
    start = now;

    std::cout << "# solutions: " << strings.size() << std::endl;
    std::cout << "Duplicates: " << solutions_including_duplicates.size() << std::endl;

    std::cout << "Time to find duplicates: " << (double)find_time / 1000.0 << " ms" << std::endl;
    std::cout << "Time to construct strings: " << (double)construct_time / 1000.0 << " ms" << std::endl;

    std::cout << "Time to print strings: " << (double)print_time / 1000.0 << " ms" << std::endl;


    std::cout << "Press any key twice to exit." << std::endl;
    std::cin.get();
    std::cin.get();

    return 0;
}

int findOneSolution(){
    Countdown countdown{{1,3,7,10,25,50}, 765};

    // START
    auto start = std::chrono::high_resolution_clock::now();

    std::string solution = countdown.findSolution();

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
            elapsed).count();

    std::cout << "Time to find one solution: " << microseconds << " us" << std::endl;
    std::cout << "Solution: " << solution << std::endl;

    return 0;
}

int main() {
    return findAllSolutions();
}