#include "big_float.hpp"

bool testFloatParse() {
    big_float<uint8_t> num = parse_big_float<uint8_t>("1000.0", 10);

    std::cout << num.binary_str() << std::endl;

    return true;
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"Parse float", testFloatParse},
    };

    try {
        for (auto[testName, runTest]: tests) {
            std::cout << "Test case: \"" << testName << '"' << std::endl;
            if (!runTest()) {
                return 1;
            }
        }

    } catch (...) {
        return 1;
    }

    return 0;
}