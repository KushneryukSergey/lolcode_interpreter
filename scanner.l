%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring> // strerror
    #include <string>
    #include <iostream>

    #include "parser.hh"

    #include "lib/driver.h"
    #include "lib/scanner.h"
    #include "lib/statement.h"
    #include "lib/var_type.h"
%}

%option noyywrap nounput noinput batch debug

%option c++
%option yyclass="Scanner"

%{
    // A number symbol corresponding to the value in S.
    yy::parser::symbol_type make_NUMBER(
        const std::string &str,
        const yy::parser::location_type& loc
    );

    yy::parser::symbol_type filter_string(
        const std::string &str,
        const yy::parser::location_type& loc
    );
%}

NAME  [a-zA-Z][a-zA-Z_0-9]*
STRING \"((\:\")|(\:\:)|(\:\))|(\:\>)|(\:o)|(\:\^)|[^"\n\:])*[\"\n]
INT   -?[1-9][0-9]*|0
SPACE [ \t\r,]

VAR   {NAME}
LIB   {NAME}

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = driver.location;
  loc.step();
%}


{SPACE}+    loc.step ();
\n+         loc.lines(yyleng); loc.step ();

"HAI"       return yy::parser::make_HAI     (loc);
"KTHXBYE"   return yy::parser::make_KTHXBYE (loc);
"VISIBLE"   return yy::parser::make_WRITE   (loc);
"GIMMEH"    return yy::parser::make_READ    (loc);

"R"         return yy::parser::make_ASSIGN  (loc);
"DIFF"      return yy::parser::make_SUB     (loc);
"SUM"       return yy::parser::make_SUM     (loc);
"PRODUKT"   return yy::parser::make_MUL     (loc);
"QUOSHUNT"  return yy::parser::make_DIV     (loc);
"MOD"       return yy::parser::make_MOD     (loc);
"BIGGR"     return yy::parser::make_MAX     (loc);
"SMALLR"    return yy::parser::make_MIN     (loc);

"UPPIN"     return yy::parser::make_INC     (loc);
"NERFIN"    return yy::parser::make_DEC     (loc);

"BOTH SAEM" return yy::parser::make_EQUALS  (loc);
"DIFFRINT"  return yy::parser::make_DIFFER  (loc);

"WIN"       return yy::parser::make_WIN     (loc);
"FAIL"      return yy::parser::make_FAIL    (loc);
"BOTH"      return yy::parser::make_AND     (loc);
"EITHER"    return yy::parser::make_OR      (loc);
"WON"       return yy::parser::make_XOR     (loc);
"NOT"       return yy::parser::make_NOT     (loc);

"IM IN YR"    return yy::parser::make_LOOP_START(loc);
"IM OUTTA YR" return yy::parser::make_LOOP_END(loc);
"TIL"       return yy::parser::make_TILL    (loc);
"WILE"      return yy::parser::make_WHILE   (loc);

"O RLY?"    return yy::parser::make_IF_START(loc);
"YA RLY"    return yy::parser::make_IF_TRUE (loc);
"NO WAI"    return yy::parser::make_IF_FALSE(loc);
"OIC"       return yy::parser::make_IF_END  (loc);

"I HAS A"   return yy::parser::make_VAR_DECL(loc);
"CAN HAS"   return yy::parser::make_INCL_LIB(loc);
"ITZ"       return yy::parser::make_ITZ     (loc);
"YR"        return yy::parser::make_YR      (loc);
"AN"        return yy::parser::make_AN      (loc);
"OF"        return yy::parser::make_OF      (loc);
"?"         return yy::parser::make_QUES    (loc);

{STRING}    return filter_string(yytext, loc);
{NAME}      return yy::parser::make_NAME(yytext, loc);
{INT}       return make_NUMBER(yytext, loc);
{INT}\.{INT} return yy::parser::make_VERSION(loc);
.           throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));
<<EOF>>     return yy::parser::make_END (loc);
%%


yy::parser::symbol_type make_NUMBER (const std::string &str,
                                     const yy::parser::location_type& loc) {
    errno = 0;
    long long n = strtol(str.c_str(), NULL, 10);
    if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
        throw yy::parser::syntax_error(loc, "integer is out of range: " + str);
    return yy::parser::make_NUMBER((int) n, loc);
}


yy::parser::symbol_type filter_string (const std::string &str,
                                       const yy::parser::location_type& loc) {
    std::string result = "";
    bool prev_deux = false;
    for (int i = 1; i < str.size() - 1; ++i) {
        if (prev_deux) {
            if (str[i] == ':') {
                result.push_back(':');
            } else if (str[i] == ')') {
                result.push_back('\n');
            } else if (str[i] == '>') {
                result.push_back('\t');
            } else if (str[i] == 'o') {
                result.push_back('\a');
            } else if (str[i] == '\"') {
                result.push_back('\"');
            } else {
                throw yy::parser::syntax_error(loc, "incorrect string: " + str);
            }
            prev_deux = false;
        } else {
            if (str[i] == ':') {
                prev_deux = true;
            } else {
                result.push_back(str[i]);
            }
        }
    }
    return yy::parser::make_STRING(result, loc);
}