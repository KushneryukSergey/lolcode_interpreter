cmake_minimum_required(VERSION 3.10)

project(Interpreter)

find_package(BISON 3.5 REQUIRED)
find_package(FLEX REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(YACC_DIR ${CMAKE_SOURCE_DIR})
set(LEX_DIR ${CMAKE_SOURCE_DIR})
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test_codes)
set(BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(HEADERS
        ${LIBRARY_DIR}/tools.h
        ${LIBRARY_DIR}/var_type.h
        ${LIBRARY_DIR}/statement.h
    )

BISON_TARGET(MyParser
        ${YACC_DIR}/parser.y
        ${CMAKE_SOURCE_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_SOURCE_DIR}/parser.hh
)

FLEX_TARGET(MyScanner
        ${LEX_DIR}/scanner.l
        ${CMAKE_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(interpreter
        ${SOURCE_DIR}/main.cpp
        ${SOURCE_DIR}/driver.cpp
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
        ${HEADERS}
        ${SOURCE_DIR}/tools.cpp
        ${SOURCE_DIR}/var_type.cpp
        ${SOURCE_DIR}/statement.cpp)

target_include_directories(interpreter PRIVATE ${CMAKE_SOURCE_DIR})

add_custom_target(clear
        COMMAND echo ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/build/*
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/scanner.cpp
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/parser.cpp
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/parser.hh
        COMMAND rm -rf ${CMAKE_SOURCE_DIR}/location.hh
        )

add_custom_target(test
        COMMAND echo ${CMAKE_BINARY_DIR}
	    COMMAND ./interpreter ${TEST_DIR}/a.in
        )



