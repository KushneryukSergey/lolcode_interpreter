//
// Created by skushneryuk on 07/02/2021.
//

#pragma once
#include <algorithm>
#include <string>

enum class UnaryOperationType {
    INC, DEC
};

enum class BinaryOperationType {
    EQUALS,
    DIFFER,
    SUM,
    SUB,
    MUL,
    DIV,
    MOD,
    MAX,
    MIN,
    AND,
    XOR,
    OR
};

enum class LoopType {
    TILL, WHILE
};

enum class Type {
    NUMBR,
    TROOF,
    YARN
};


// functions

std::string TROOF_TO_STRING(const bool&);

