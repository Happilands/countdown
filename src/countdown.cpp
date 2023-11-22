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

bool Countdown::hasSolution() {
    if(std::find(m_Start.begin(), m_Start.end(), m_Result) != m_Start.end())
        return true;

    auto pos = m_Start;
    return hasSolution(pos);
}

std::vector<Solution>& Countdown::findSolutions() {
    auto pos = m_Start;
    return findSolutions(pos);
}

int32_t Countdown::findSolutionCount() {
    auto pos = m_Start;

    countSolutions(pos);

    return m_Numbers2SolutionCount[pos];
}

int32_t Countdown::countSolutions(Position &position) {
    auto pair = m_Numbers2SolutionCount.insert(std::make_pair(position, 0));

    int32_t& solutionCount = pair.first->second;
    bool& wasInserted = pair.second;

    // If position couldn't be inserted look up the value in the table
    if(!wasInserted)
        return solutionCount;

    // Item was inserted, now find m_Solutions
    for(auto it = position.begin(); it != position.end() - 1; it++){
        for(auto jt = it + 1; jt != position.end(); jt++){
            int32_t n = *it;
            int32_t m = *jt;

            position.erase(jt);
            position.erase(it);

            auto doOperation = [this, &solutionCount, &position](OperationType op, int32_t res) -> void {
                int solutionMultiplier = 1 + ((op == Addition) || (op == Multiplication));

                if(res == m_Result) {
                    solutionCount += solutionMultiplier;
                    return;
                }

                auto kt = position.begin();
                while (kt != position.end() && res < *kt)
                    kt++;

                position.insert(kt, res);

                solutionCount += countSolutions(position) * solutionMultiplier;

                position.erase(kt);
            };

            doOperation(Addition, n + m);
            doOperation(Multiplication, n * m);
            if(n != m)
                doOperation(Subtraction, n - m);
            if(n % m == 0)
                doOperation(Division, n / m);

            position.insert(it, n);
            position.insert(jt, m);
        }
    }

    return solutionCount;
}

bool Countdown::hasSolution(Position& position) {
    for(auto it = position.begin(); it != position.end() - 1; it++){
        for(auto jt = it + 1; jt != position.end(); jt++){
            int32_t n = *it;
            int32_t m = *jt;

            position.erase(jt);
            position.erase(it);

            auto doOperation = [this, &position](OperationType op, int32_t res) -> bool {
                if(res == m_Result)
                    return true;

                auto kt = position.begin();
                while (kt != position.end() && res < *kt)
                    kt++;

                position.insert(kt, res);

                if(hasSolution(position))
                    return true;

                position.erase(kt);
                return false;
            };

            if(doOperation(Addition, n + m))
                return true;
            if(doOperation(Multiplication, n * m))
                return true;
            if(doOperation(Subtraction, n - m))
                return true;
            if(m != 0 && n % m == 0 && doOperation(Division, n / m))
                return true;

            position.insert(it, n);
            position.insert(jt, m);
        }
    }
    return false;
}

std::vector<Solution>& Countdown::findSolutions(Position &position) {
    auto iter = m_Numbers2solutions.find(position);
    if(iter != m_Numbers2solutions.end())
        return iter->second;
    iter = m_Numbers2solutions.insert(std::make_pair(position, std::vector<Solution>())).first;
    auto& solutionList = iter->second;

    for(int i = 0; i < position.size() - 1; i++){
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

    int opIndex = 0;
    while (opIndex < 5 && !solution.operations[opIndex].isZero()){
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

        auto kt = numbers.begin();
        while (kt != numbers.end() && res < (*kt).first)
            kt++;

        numbers.insert(kt, std::make_pair(res, ss.str()));
        opIndex++;
    }
    return numbers[0].second;
}

void Countdown::removeDuplicatesAndConvertToStrings(const std::vector<Solution> &solutions, std::unordered_set<std::string> &output) {
    for(const Solution& solution : solutions){
        output.insert(solutionToString(solution));
    }
}