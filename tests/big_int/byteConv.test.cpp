#include <iostream>
#include <vector>
#include <bitset>

#include "BigIntBackend.h"
#include "../utils.h"

using namespace BigNumbers;

void print(std::ostream &out, unsigned char *value, std::size_t size) {
    if (value == nullptr) {
        out << "null";

        return;
    }

    for (std::size_t i = 0; i < size; ++i) {
        std::bitset<8> buffer(value[i]);
        out << buffer.to_string();
    }
}

bool compare(unsigned char *expected, std::size_t expectedSize, unsigned char *received, std::size_t receivedSize) {
    if (expectedSize != receivedSize) {
        std::cout << "Lengths not match: \nExpected: ";
        print(std::cout, expected, expectedSize);
        std::cout << "\nReceived: ";
        print(std::cout, received, receivedSize);
        std::cout << std::endl;

        return false;
    }

    if (strncmp(reinterpret_cast<char *>(expected), reinterpret_cast<char *>(received), expectedSize) != 0) {
        std::cout << "Values not match: \nExpected: ";
        print(std::cout, expected, expectedSize);
        std::cout << "\nReceived: ";
        print(std::cout, received, receivedSize);
        std::cout << std::endl;

        return false;
    }

    return true;
}

bool testOnePieceToBytes() {
    BigIntBackend<uint8_t> b(false, {0b00001111});

    auto bytesArray = b.getBytes();
    unsigned char expected[] = {0b00001111};
    std::size_t expectedSize = sizeof(expected) / sizeof(unsigned char);

    return compare(expected, expectedSize, bytesArray.first, bytesArray.second);
}

bool testMultiplePiecesToBytes() {
    BigIntBackend<uint8_t> b(false, {0b01000011, 0b11001100, 0b00001111});

    auto bytesArray = b.getBytes();

    unsigned char expected[] = {0b01000011, 0b11001100, 0b00001111};

    std::size_t expectedSize = sizeof(expected) / sizeof(unsigned char);

    return compare(expected, expectedSize, bytesArray.first, bytesArray.second);
}

bool testWidePiecesToBytes() {
    BigIntBackend<uint64_t> b(false, {0b0100001111001100000011110000111101000011110011000000111100001111});

    auto bytesArray = b.getBytes();

    unsigned char expected[] = {0b01000011, 0b11001100, 0b00001111, 0b00001111,
                                0b01000011, 0b11001100, 0b00001111, 0b00001111};

    std::size_t expectedSize = sizeof(expected) / sizeof(unsigned char);

    return compare(expected, expectedSize, bytesArray.first, bytesArray.second);
}

int main() {
    using test = bool (*)();

    std::vector<std::pair<std::string, test>> tests{
            {"One piece",                 testOnePieceToBytes},
            {"Multiple pieces to bytes",  testMultiplePiecesToBytes},
            {"Test wide pieces to bytes", testWidePiecesToBytes}
    };

    return runTests(tests);
}