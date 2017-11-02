#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

int main() {
    std::cout << 
        "***************************************************\n"
        "***             REMOTE FACTORIZATION            ***\n"
        "***************************************************\n"
        "\n" << std::endl << "Just type your number: " << std::flush;


    std::string s;
    std::cin >> s;
    s = "cd actual; factor " + s;
    std::cout << "It was easy! Factorization of " << std::flush;
    system(s.c_str());
    std::cout << std::flush;

    usleep(200);
}

