%skeleton "lalr1.cc"
%require "3.5"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    #include <iostream>
    #include <algorithm>

    class Statement;
    class StmtList;
    class ExprStmt;
    class VarStmt;
    class NotStmt;
    class UnaryStmt;
    class BinaryStmt;
    class ConstStmt;
    class AssignStmt;
    class LoopStmt;
    class IfElseStmt;
    class WriteStmt;
    class ReadStmt;

    enum class UnaryOperationType;
    enum class BinaryOperationType;
    enum class LoopType;
    enum class Type;

    class Scanner;
    class Driver;
}

%define parse.trace
%define parse.error verbose

%code {
    #include "lib/driver.h"
    #include "lib/scanner.h"
    #include "lib/statement.h"
    #include "lib/var_type.h"
    #include "lib/tools.h"
    #include "location.hh"
    #include "parser.hh"

    class Scanner;
    class Driver;

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%locations

%define api.token.prefix {TOK_}

%token
    END 0       "end of file"

// mathematical
    ASSIGN      "R"
    SUM         "SUM"
    SUB         "DIFF"
    MUL         "PRODUKT"
    DIV         "QUOSHUNT"
    MOD         "MOD"
    MAX         "BIGGR"
    MIN         "SMALLR"

// unary
    INC         "UPPIN"
    DEC         "NERFIN"
// comp
    EQUALS      "BOTH SAEM"
    DIFFER      "DIFFRINT"

// logical
    WIN         "WIN"
    FAIL        "FAIL"
    AND         "BOTH"
    OR          "EITHER"
    XOR         "WON"
    NOT         "NOT"

// for and while
    LOOP_START  "IM IN YR"
    LOOP_END    "IM OUTTA YR"
    TILL        "TIL"
    WHILE       "WILE"

// if else
    IF_START    "O RLY?"
    IF_TRUE     "YA RLY"
    IF_FALSE    "NO WAI"
    IF_END      "OIC"

// special
    HAI         "HAI"
    KTHXBYE     "KTHXBYE"
    WRITE       "VISIBLE"
    READ        "GIMMEH"
    VERSION     "VERSION"

    VAR_DECL    "I HAS A"
    ITZ         "ITZ"

    INCL_LIB "CAN HAS"
    QUES        "?"

    YR          "YR"
    AN          "AN"
    OF          "OF"
;

%token <std::string> NAME "identifier or library"
%token <std::string> STRING "string"
%token <int> NUMBER "number"

%nterm <StmtList*> STMTS
%nterm <ExprStmt*> EXPR
%nterm <AssignStmt*> TOP_LEVEL_EXPR
%nterm <AssignStmt*> DECLARATION
%nterm <AssignStmt*> ASSIGNMENT
%nterm <UnaryStmt*> UNARY_STMT
%nterm <IfElseStmt*> CONDITION
%nterm <WriteStmt*> OUTPUT
%nterm <ReadStmt*> INPUT
%nterm <LoopStmt*> LOOP


%printer { yyo << $$; } <*>;

%%
%start PROGRAM;
PROGRAM:
    HAI VERSION STMTS KTHXBYE { driver.program = $3; }
    | HAI STMTS KTHXBYE { driver.program = $2; }

STMTS:
    %empty {
        $$ = new StmtList();
    }
    | STMTS TOP_LEVEL_EXPR { $$ = $1; $1->push_back($2); }
    | STMTS DECLARATION { $$ = $1; $1->push_back($2); }
    | STMTS ASSIGNMENT { $$ = $1; $1->push_back($2); }
    | STMTS UNARY_STMT { $$ = $1; $1->push_back($2); }
    | STMTS CONDITION { $$ = $1; $1->push_back($2); }
    | STMTS INCLUDE { $$ = $1; }
    | STMTS OUTPUT { $$ = $1; $1->push_back($2); }
    | STMTS INPUT { $$ = $1; $1->push_back($2); }
    | STMTS LOOP { $$ = $1; $1->push_back($2); }
    ;

EXPR:
// constants and variables
    NAME {
        $$ = new VarStmt(driver.variables[$1]);
    }
    | WIN {
          $$ = new ConstStmt(VarType(true));
      }
    | FAIL {
          $$ = new ConstStmt(VarType(false));
      }
    | STRING {
          $$ = new ConstStmt(VarType($1));
      }
    | NUMBER {
          $$ = new ConstStmt(VarType($1));
      }
// comparison
    | EQUALS EXPR AN EXPR {
        $$ = new BinaryStmt($2, $4, BinaryOperationType::EQUALS);
      }
    | DIFFER EXPR AN EXPR {
        $$ = new BinaryStmt($2, $4, BinaryOperationType::DIFFER);
      }
// logical expression
    | NOT EXPR {
          $$ = new NotStmt($2);
      }
    | AND OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::AND);
      }
    | XOR OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::XOR);
      }
    | OR  OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::OR);
      }
// mathematical expression
    | SUM OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::SUM);
      }
    | SUB OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::SUB);
      }
    | MUL OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::MUL);
      }
    | DIV OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::DIV);
      }
    | MOD OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::MOD);
      }
    | MAX OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::MAX);
      }
    | MIN OF EXPR AN EXPR {
          $$ = new BinaryStmt($3, $5, BinaryOperationType::MIN);
      }
    ;

TOP_LEVEL_EXPR:
    EXPR {
        $$ = new AssignStmt($1, driver.variables["IT"]);
    }
    ;

DECLARATION:
    VAR_DECL NAME {
        $$ = new AssignStmt(new ConstStmt(VarType(0)), driver.variables[$2]);
    }
    | VAR_DECL NAME ITZ EXPR {
          $$ = new AssignStmt($4, driver.variables[$2]);
      }
    ;

ASSIGNMENT:
    NAME ASSIGN EXPR {
        $$ = new AssignStmt($3, driver.variables[$1]);
    }
    ;

UNARY_STMT:
    INC YR NAME {
        $$ = new UnaryStmt(UnaryOperationType::INC, driver.variables[$3]);
    }
    | DEC YR NAME {
          $$ = new UnaryStmt(UnaryOperationType::DEC, driver.variables[$3]);
      }
    ;

CONDITION:
    IF_START IF_TRUE STMTS IF_END {
        $$ = new IfElseStmt($3, new StmtList(), driver.variables["IT"]);
    }
    | IF_START IF_TRUE STMTS IF_FALSE STMTS IF_END {
          $$ = new IfElseStmt($3, $5, driver.variables["IT"]);
      }
    ;

INCLUDE:
    INCL_LIB NAME QUES {}
    ;

OUTPUT:
    WRITE EXPR { $$ = new WriteStmt($2); }
    ;

INPUT:
    READ NAME { $$ = new ReadStmt(driver.variables[$2]); }
    ;

LOOP:
    LOOP_START NAME UNARY_STMT WHILE EXPR STMTS LOOP_END NAME {
        $$ = new LoopStmt($3, $5, $6, LoopType::WHILE);
    }
    | LOOP_START NAME UNARY_STMT TILL EXPR STMTS LOOP_END NAME {
          $$ = new LoopStmt($3, $5, $6, LoopType::WHILE);
      }
    ;


%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
