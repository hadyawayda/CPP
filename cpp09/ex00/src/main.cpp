// ex00/main.cpp
#include "BitcoinExchange.hpp"
#include <iostream>

int main(int argc, char** argv) {
    try {
        // The subject provides the CSV database of historical BTC prices. :contentReference[oaicite:2]{index=2}
        const std::string dbPath = "data.csv";
        BitcoinExchange btc(dbPath);

        if (argc != 2) {
            std::cout << "Error: could not open file." << std::endl;
            return 0;
        }
        btc.processInputFile(argv[1]);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
