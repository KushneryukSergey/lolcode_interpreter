#pragma once

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif


#include "../parser.hh"
#include "../location.hh"

#undef YY_DECL
#define YY_DECL yy::parser::symbol_type Scanner::ScanToken()

class Driver;

class Scanner: public yyFlexLexer {
 public:
    Scanner(Driver& driver): driver(driver) {}
	virtual ~Scanner() {}
	virtual yy::parser::symbol_type ScanToken();
    Driver &driver;
};