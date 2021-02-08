//
// Created by skushneryuk on 07/02/2021.
//

#include "../lib/var_type.h"

VarType::VarType(Type type): _type(type) {
    if (_type == Type::NUMBR) {
        _value = int();
    }
    if (_type == Type::YARN) {
        _value = string();
    }
    if (_type == Type::TROOF) {
        _value = bool();
    }
}

bool VarType::to_bool() const {
    if (_type == Type::NUMBR) {
        return std::get<int>(_value);
    } else if (_type == Type::YARN) {
        return !std::get<string>(_value).empty();
    } else if (_type == Type::TROOF) {
        return std::get<bool>(_value);
    }
    throw std::bad_cast();
}

string VarType::to_string() const {
    if (_type == Type::NUMBR) {
        return std::to_string(std::get<int>(_value));
    } else if (_type == Type::YARN) {
        return std::get<string>(_value);
    } else if (_type == Type::TROOF) {
        return TROOF_TO_STRING(std::get<bool>(_value));
    }
    throw std::bad_cast();
}

int VarType::to_int() const {
    if (_type == Type::NUMBR) {
        return std::get<int>(_value);
    } else if (_type == Type::YARN) {
        return std::stoi(std::get<string>(_value));
    } else if (_type == Type::TROOF) {
        return std::get<bool>(_value);
    }
    throw std::bad_cast();
}

VarType VarType::operator+(const VarType& other) const {
    return VarType(to_int() + other.to_int());
}

VarType VarType::operator-(const VarType& other) const {
    return VarType(to_int() - other.to_int());
}

VarType VarType::operator*(const VarType& other) const {
    return VarType(to_int() * other.to_int());
}

VarType VarType::operator/(const VarType& other) const {
    return VarType(to_int() / other.to_int());
}

VarType VarType::operator%(const VarType& other) const {
    return VarType(to_int() % other.to_int());
}

VarType VarType::operator&(const VarType& other) const {
    return VarType(to_bool() & other.to_bool());
}

VarType VarType::operator^(const VarType& other) const {
    return VarType(to_bool() ^ other.to_bool());
}

VarType VarType::operator|(const VarType& other) const {
    return VarType(to_bool() | other.to_bool());
}

VarType& VarType::operator++() {
    _value = to_int() + 1;
    return *this;
}

VarType& VarType::operator--() {
    _value = to_int() - 1;
    return *this;
}

VarType VarType::operator!() const {
    return VarType(!to_bool());
}

bool VarType::operator==(const VarType& other) const {
    return _value == other._value;
}

bool VarType::operator!=(const VarType& other) const {
    return !(*this == other);
}

VarType max(const VarType& first, const VarType& second) {
    return VarType(std::max(first.to_int(), second.to_int()));
}

VarType min(const VarType& first, const VarType& second) {
    return VarType(std::min(first.to_int(), second.to_int()));
}