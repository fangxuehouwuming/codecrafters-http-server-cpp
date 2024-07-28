#include <iostream>
#include "include/server.hpp"

int main(int argc, char** argv) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "Logs from your program will appear here!\n";

    Server server(argc, argv);
    server.Run();

    return 0;
}