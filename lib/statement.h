//
// Created by skushneryuk on 07/02/2021.
//

#pragma once

#include <vector>
#include <string>
#include "var_type.h"
#include "tools.h"
#include <map>
#include <iostream>

using std::vector;
using std::string;
using std::map;

class Statement;

class StmtList {
    vector<Statement*> _list;
public:
    StmtList() = default;
    
    StmtList& operator= (StmtList&& other) noexcept;
    
    void push_back(Statement*);
    void execute();
    ~StmtList();
};


// Statements

class Statement {
public:
    virtual void execute() = 0;
    virtual ~Statement() = default;
};

// Expression statement
class ExprStmt : public Statement {
protected:
    VarType _value;
public:
    ExprStmt(const VarType& value) : _value(value) {}
    ExprStmt() {}
    virtual void execute() = 0;
    virtual ~ExprStmt() = default;
    
    virtual const VarType& get_result();
};

// Types of expression statements
class VarStmt : public ExprStmt {
protected:
    VarType& _ref;
public:
    VarStmt(VarType&);
    
    virtual void execute();
    virtual ~VarStmt() = default;
};

class NotStmt : public ExprStmt {
    ExprStmt* _expr;
public:
    NotStmt(ExprStmt*&);
    
    virtual void execute();
    virtual ~NotStmt();
};

class UnaryStmt : public Statement {
    UnaryOperationType _type;
    VarType& _ref;
public:
    UnaryStmt(UnaryOperationType, VarType&);
    
    virtual void execute();
    virtual ~UnaryStmt() = default;
};

class BinaryStmt : public ExprStmt {
    ExprStmt* _left_expr;
    ExprStmt* _right_expr;
    BinaryOperationType _type;
public:
    BinaryStmt(ExprStmt*&, ExprStmt*&, BinaryOperationType);
    
    virtual void execute();
    virtual ~BinaryStmt();
};

class ConstStmt : public ExprStmt {
public:
    ConstStmt(const VarType&);
    
    virtual void execute();
    virtual ~ConstStmt() = default;
};


class AssignStmt : public Statement {
protected:
    ExprStmt* _stmt;
    VarType& _ref;
public:
    AssignStmt(ExprStmt*, VarType&);
    
    virtual void execute();
    virtual ~AssignStmt();
};

/*
class TopLevelExprStmt : public AssignStmt {
public:
    TopLevelExprStmt(ExprStmt*);
    
    //virtual void execute() = 0;
    virtual ~TopLevelExprStmt() = 0;
};
*/

class LoopStmt: public Statement {
protected:
    UnaryStmt* _chng;
    ExprStmt* _cond;
    StmtList* _stmts;
    LoopType _type;
public:
    LoopStmt(UnaryStmt*, ExprStmt*, StmtList*, LoopType);
    
    virtual void execute();
    virtual ~LoopStmt();
};

class IfElseStmt : public Statement {
protected:
    VarType& _ref;
    StmtList* _true_stmts;
    StmtList* _false_stmts;
public:
    IfElseStmt(StmtList*, StmtList*, VarType&);
    
    virtual void execute();
    virtual ~IfElseStmt();
};

class WriteStmt : public Statement {
    ExprStmt* _stmt;
public:
    WriteStmt(ExprStmt*&);
    
    virtual void execute();
    virtual ~WriteStmt();
};


class ReadStmt : public Statement {
    VarType& _ref;
public:
    ReadStmt(VarType&);
    virtual void execute();
    virtual ~ReadStmt() = default;
};
