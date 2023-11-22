//
// Created by daanp on 20/11/2023.
//

#include "countdown.h"
#include <algorithm>
#include <iostream>
#include <sstream>

Countdown::Countdown(const Position& _start, int32_t result) {
    if(_start.size() > 8)
        throw std::invalid_argument("Countdown::Countdown only accepts up to eight starting numbers");
    for(int32_t i : _start) if(i <= 0)
        throw std::invalid_argument("Countdown::Countdown only accepts strictly positive integers");
    if(result <= 0)
        throw std::invalid_argument("Countdown::Countdown only accepts strictly positive integers");

    this->m_Start = _start;
    this->m_Result = result;

    std::sort(m_Start.rbegin(), m_Start.rend());
}

std::vector<Solution>& Countdown::findSolutions() {
    auto pos = m_Start;
    return findSolutions(pos);
}

std::vector<Solution>& Countdown::findSolutions(Position &position) {
    auto iter = m_Numbers2solutions.find(position);
    if(iter != m_Numbers2solutions.end())
        return iter->second;
    iter = m_Numbers2solutions.insert(std::make_pair(position, std::vector<Solution>())).first;
    auto& solutionList = iter->second;

    for(int i = 0; i + 1 < position.size(); i++){
        for(int j = i + 1; j < position.size(); j++){
            int32_t n = position[i];
            int32_t m = position[j];

            position.erase(position.begin() + j);
            position.erase(position.begin() + i);

            auto doOperation = [i, j, this, &solutionList, &position](OperationType operationType, int32_t res) -> void {
                Operation operation = Operation(operationType, i, j);

                if(res == m_Result) {
                    solutionList.emplace_back(operation);
                    if(operationType == Addition || operationType == Multiplication)
                        solutionList.emplace_back(Operation(operationType, j, i));
                }

                int k = 0;
                while (k < position.size() && res < position[k])
                    k++;

                position.insert(position.begin() + k, res);

                for(Solution solution : findSolutions(position)){
                    if(operationType == Addition || operationType == Multiplication){
                        Solution s2 = solution;
                        s2.prependOperation(Operation(operationType, j, i));
                        solutionList.push_back(s2);
                    }
                    solution.prependOperation(operation);
                    solutionList.push_back(solution);
                }

                position.erase(position.begin() + k);
            };

            doOperation(Addition, n + m);
            doOperation(Multiplication, n * m);
            if(n != m)
                doOperation(Subtraction, n - m);
            if(n % m == 0)
                doOperation(Division, n / m);

            position.insert(position.begin() + i, n);
            position.insert(position.begin() + j, m);
        }
    }

    return solutionList;
}

const char* operation_to_string(OperationType op){
    switch (op) {
        case Addition:
            return "+";
        case Multiplication:
            return "*";
        case Division:
            return "/";
        case Subtraction:
            return "-";
    }
    return ".";
}

int do_operation(OperationType op, int n, int m){
    switch (op) {
        case Addition:
            return n + m;
        case Multiplication:
            return n * m;
        case Division:
            return n / m;
        case Subtraction:
            return n - m;
    }
    return -1;
}

std::string Countdown::solutionToString(Solution solution){
    std::vector<std::pair<int, std::string>> numbers;

    for(int i : m_Start){
        numbers.emplace_back(i, std::to_string(i));
    }

    int resIndex = -1;

    int opIndex = 0;
    while (opIndex < 8 && !solution.operations[opIndex].isZero()){
        Operation operation = solution.operations[opIndex];

        int i = operation.getFirst();
        int j = operation.getSecond();

        std::string opString = operation_to_string(operation.getOperation());

        std::stringstream ss;

        ss << "(" << numbers[i].second << opString << numbers[j].second << ")";

        int res = do_operation(operation.getOperation(), numbers[i].first, numbers[j].first);

        if(j > i)
            numbers.erase(numbers.begin() + j);
        numbers.erase(numbers.begin() + i);
        if(j <= i)
            numbers.erase(numbers.begin() + j);

        resIndex = 0;
        while (resIndex < numbers.size() && res < numbers[resIndex].first)
            resIndex++;

        numbers.insert(numbers.begin() + resIndex, std::make_pair(res, ss.str()));
        opIndex++;
    }
    return numbers[resIndex].second;
}

void Countdown::removeDuplicatesAndConvertToStrings(const std::vector<Solution> &solutions, std::unordered_set<std::string> &output) {
    for(const Solution& solution : solutions){
        output.insert(solutionToString(solution));
    }
}