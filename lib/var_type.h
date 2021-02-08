//
// Created by skushneryuk on 07/02/2021.
//

#pragma once

#include <variant>


#include <variant>
#include <iostream>
#include <string>
#include <exception>
#include "tools.h"

using std::string;

class VarType {
private:
    Type _type;
    std::variant<int, bool, string> _value;

public:
    explicit VarType(Type type = Type::NUMBR);
    VarType(const VarType&) = default;
    
    VarType& operator=(const VarType& other) = default;
    VarType& operator=(VarType&& other) = default;
    
    VarType operator+(const VarType&) const;
    VarType operator-(const VarType&) const;
    VarType operator*(const VarType&) const;
    VarType operator/(const VarType&) const;
    VarType operator%(const VarType&) const;
    VarType operator&(const VarType&) const;
    VarType operator^(const VarType&) const;
    VarType operator|(const VarType&) const;
    VarType operator!() const;
    
    VarType& operator++();
    VarType& operator--();
    
    bool operator==(const VarType&) const;
    bool operator!=(const VarType&) const;
    
    
    explicit VarType(const string& value) : _type(Type::YARN), _value(value) {}
    explicit VarType(const int& value) : _type(Type::NUMBR), _value(value) {}
    explicit VarType(const bool& value) : _type(Type::TROOF), _value(value) {}
    
    bool    to_bool() const;
    string  to_string() const;
    int     to_int() const;
};

VarType max(const VarType&, const VarType&);

VarType min(const VarType&, const VarType&);