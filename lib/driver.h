#pragma once

#include <map>
#include <string>
#include <fstream>
#include "scanner.h"
#include "../parser.hh"
#include "statement.h"
#include "var_type.h"

class Driver {
public:
    bool trace_parsing;
    bool trace_scanning;
    
    std::map<std::string, VarType> variables;
    std::string file;
    StmtList* program;
    
    friend class Scanner;
    Scanner scanner;
    yy::parser parser;
    yy::location location;
    
    
    Driver();
    int parse(const std::string& f);

    void scan_begin();
    void scan_end();

 private:
    std::ifstream stream;
};
