//
// Created by skushneryuk on 07/02/2021.
//

#include "../lib/statement.h"

void StmtList::execute() {
    for (auto statement: _list) {
        statement->execute();
    }
}

StmtList::~StmtList() {
    for (auto statement: _list) {
        delete statement;
    }
}

StmtList& StmtList::operator=(StmtList&& other) noexcept {
    _list = std::move(other._list);
    other._list.clear();
    return *this;
}

void StmtList::push_back(Statement* stmt) {
    _list.push_back(stmt);
}

const VarType& ExprStmt::get_result() {
    return _value;
}

VarStmt::VarStmt(VarType& ref): _ref(ref) {}
    
void VarStmt::execute() {
    _value = _ref;
}

NotStmt::NotStmt(ExprStmt*& expr): _expr(expr) {}

void NotStmt::execute() {
    _expr->execute();
    _value = !(_expr->get_result());
}

NotStmt::~NotStmt() {
    delete _expr;
}

UnaryStmt::UnaryStmt(UnaryOperationType type,
                     VarType& ref):
                        _type(type),
                        _ref(ref) {}

void UnaryStmt::execute() {
    if (_type == UnaryOperationType::INC) {
        ++_ref;
    } else {
        --_ref;
    }
}


BinaryStmt::BinaryStmt(ExprStmt*& left,
                       ExprStmt*& right,
                       BinaryOperationType type):
                           _left_expr(left),
                           _right_expr(right),
                           _type(type) {}

void BinaryStmt::execute() {
    _left_expr->execute();
    _right_expr->execute();
    if (_type == BinaryOperationType::EQUALS) {
        _value = VarType(_left_expr->get_result() == _right_expr->get_result());
    } else if (_type == BinaryOperationType::DIFFER) {
        _value = VarType(_left_expr->get_result() != _right_expr->get_result());
    } else if (_type == BinaryOperationType::MAX) {
        _value = max(_left_expr->get_result(), _right_expr->get_result());
    } else if (_type == BinaryOperationType::MIN) {
        _value = min(_left_expr->get_result(), _right_expr->get_result());
    } else if (_type == BinaryOperationType::SUM) {
        _value = _left_expr->get_result() + _right_expr->get_result();
    } else if (_type == BinaryOperationType::SUB) {
        _value = _left_expr->get_result() - _right_expr->get_result();
    } else if (_type == BinaryOperationType::MUL) {
        _value = _left_expr->get_result() * _right_expr->get_result();
    } else if (_type == BinaryOperationType::DIV) {
        _value = _left_expr->get_result() / _right_expr->get_result();
    } else if (_type == BinaryOperationType::MOD) {
        _value = _left_expr->get_result() % _right_expr->get_result();
    } else if (_type == BinaryOperationType::AND) {
        _value = _left_expr->get_result() & _right_expr->get_result();
    } else if (_type == BinaryOperationType::XOR) {
        _value = _left_expr->get_result() ^ _right_expr->get_result();
    } else if (_type == BinaryOperationType::OR) {
        _value = _left_expr->get_result() | _right_expr->get_result();
    }
}

BinaryStmt::~BinaryStmt() {
    delete _left_expr;
    delete _right_expr;
}


ConstStmt::ConstStmt(const VarType& value): ExprStmt(value) {}

void ConstStmt::execute() {}


AssignStmt::AssignStmt(ExprStmt* stmt, VarType& ref): _stmt(stmt), _ref(ref) {}

void AssignStmt::execute() {
    _stmt->execute();
    _ref = _stmt->get_result();
}

AssignStmt::~AssignStmt() {
    delete _stmt;
}


LoopStmt::LoopStmt(UnaryStmt* chng,
                   ExprStmt* cond,
                   StmtList* stmts,
                   LoopType type):
                       _chng(chng),
                       _cond(cond),
                       _stmts(stmts),
                       _type(type) {}

void LoopStmt::execute() {
    _cond->execute();
    while (_cond->get_result().to_bool() ^ (_type == LoopType::TILL)) {
        _stmts->execute();
        _chng->execute();
        _cond->execute();
    }
}

LoopStmt::~LoopStmt() {
    delete _chng;
    delete _cond;
    delete _stmts;
}


IfElseStmt::IfElseStmt(StmtList* true_stmts,
                       StmtList* false_stmts,
                       VarType& ref) :
                           _ref(ref),
                           _true_stmts (true_stmts),
                           _false_stmts(false_stmts){}

void IfElseStmt::execute() {
    if (_ref.to_bool()) {
        _true_stmts->execute();
    } else {
        _false_stmts->execute();
    }
}

IfElseStmt::~IfElseStmt() {
    delete _true_stmts;
    delete _false_stmts;
}


WriteStmt::WriteStmt(ExprStmt*& stmt): _stmt(stmt) {}

void WriteStmt::execute() {
    _stmt->execute();
    std::cout << _stmt->get_result().to_string() << std::endl;
}

WriteStmt::~WriteStmt() {
    delete _stmt;
}


ReadStmt::ReadStmt(VarType& ref): _ref(ref) {}

void ReadStmt::execute() {
    string str;
    std::cin >> str;
    _ref = VarType(str);
}

