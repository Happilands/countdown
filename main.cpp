#include <iostream>

#include "src/countdown.h"

#include <ctime>

int main() {
    Countdown countdown{{1, 3, 7, 10, 25, 50}, 765};

    // START
    std::clock_t    start;
    start = std::clock();

    auto& solutions_including_duplicates = countdown.findSolutions();
    std::unordered_set<std::string> strings;
    countdown.removeDuplicatesAndConvertToStrings(solutions_including_duplicates, strings);

    // AAAAND TIME!
    double time = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);

    // I don't count the time it takes to print the strings
    for(const auto & string : strings){
        std::cout << string << std::endl;
    }

    std::cout << "# solutions: " << strings.size() << std::endl;

    std::cout << "Time: " << time << " ms" << std::endl;

    return 0;
}