#include <iostream>
#include "../lib/driver.h"

int main(int argc, char** argv) {
    int result = 0;
    Driver driver;
    
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == std::string("-p")) {
            driver.trace_parsing = true;
        } else if (argv[i] == std::string("-s")) {
            driver.trace_scanning = true;
        } else if (!driver.parse(argv[i])) {
            driver.program->execute();
            delete driver.program;
            driver.program = nullptr;
            std::cout << "\n"
                         "__________________________\n"
                         "Program ended successfully" << std::endl;
        } else {
            result = 1;
        }
    }

    return result;
}