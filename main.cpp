#include <iostream>
#include <getopt.h>

#include "Mint.h"

auto helper() {
    std::cout << "Mint interpreter. Usage: \n" <<
                 "-f, --file [FILE]   | Run a Mint script\n" <<
                 "-a, --about         | About Mint\n" <<
                 "-h, --help          | Show this helper\n"
                 "Run Mint without parameters to open the REPL." << std::endl;
}

int main(int argc, char **argv) {
    if(argc > 3) {
        std::cerr << "Error: too many arguments. Please use \"" << argv[0] << " --help\" to show the helper." << std::endl;
        return 1;
    }

    int opt;
    const char * short_opts = "f:ah";
    std::string file_name;
    auto execute_from_file = false;
    struct option long_opts[] = {
            {"file", required_argument, nullptr, 'f'},
            {"about", no_argument, nullptr, 'a'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, 0, nullptr, 0}
    };

    while((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1) {
        switch (opt) {
            case 'f': {
                file_name = std::string(optarg);
                execute_from_file = true;
            }
            break;
            case 'a': {
                std::cout << "Mint is an interpreted programming language written in C++.\n"
                          << "For further information, please refer to https://github.com/ice-bit/Mint\n"
                          << "(c) 2022 Marco Cetica <email@marcocetica.com>" << std::endl;
                return 0;
            }
            default: case 'h': case ':': case '?': helper(); return 0;;
        }
    }

    auto ret = 0;
    if(execute_from_file)
        ret = Mint::run_file(file_name);
    else
        Mint::run_prompt();


    return ret;
}