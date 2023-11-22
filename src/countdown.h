//
// Created by daanp on 20/11/2023.
//

#ifndef COUNTDOWN_COUNTDOWN_H
#define COUNTDOWN_COUNTDOWN_H

#include <vector>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <string>

enum OperationType : char {
    Addition,
    Multiplication,
    Subtraction,
    Division
};

struct Operation{
private:
    OperationType operation : 2;
    char i : 3;
    char j : 3;

public:
    Operation(OperationType operationType, char i, char j)
        : operation(operationType), i(i), j(j){
    }

    OperationType getOperation() const{
        return (OperationType)(getBits() & 0b00000011);
    }

    int getFirst() const{
        return (getBits() & 0b00011100) >> 2;
    }

    int getSecond() const{
        return (getBits() & 0b11100000) >> 5;
    }

    bool isZero() const{
        // Cast the bitfield to a char and check if it is zero
        return getBits() == 0;
    }

    uint8_t getBits() const{
        return *((uint8_t*)this);
    }
};

struct Solution{
    union {
        Operation operations[8];
        uint64_t bits;
    };

public:
    Solution() : bits(0) {
    }

    explicit Solution(Operation operation){
        bits = operation.getBits();
    }

    explicit operator uint64_t () const { return *((uint64_t*)this); }

    void prependOperation(Operation op){
        bits <<= 8;
        bits |= op.getBits();
    }
};

using Position = std::vector<int32_t>;

template<> struct std::hash<std::vector<int32_t>> {
    auto operator() (const std::vector<int32_t>& key) const {
        std::hash<int32_t> hasher;
        size_t result = 0;
        for(int i : key) {
            result = (result << 1) ^ hasher(i); // ??
        }
        return result;
    }
};

class Countdown {
public:
    explicit Countdown(const Position& start, int32_t result);

    std::vector<Solution>& findSolutions();
    bool hasSolution();
    int32_t findSolutionCount();

public:
    void removeDuplicatesAndConvertToStrings(const std::vector<Solution>& solutions, std::unordered_set<std::string>& output);

private:
    std::vector<Solution>& findSolutions(std::vector<int32_t>& operationType);
    bool hasSolution(std::vector<int32_t>& position);
    int32_t countSolutions(std::vector<int32_t>& position);
    std::string solutionToString(Solution solution);

private:
    int32_t m_Result = -1;
    Position m_Start;

    std::unordered_map<Position, std::vector<Solution>> m_Numbers2solutions;
    std::unordered_map<Position, int32_t> m_Numbers2SolutionCount;
};

#endif //COUNTDOWN_COUNTDOWN_H
