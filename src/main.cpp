#include <iostream>

#include "Mint.h"

int main(int argc, char **argv) {
    if(argc > 2)
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
    else if(argc == 2)
        Mint::run_file(argv[1]);
    else
        Mint::run_prompt();

    return 0;
}