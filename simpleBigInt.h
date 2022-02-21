#ifndef BIG_NUMBERS_SIMPLEBIGINT_H
#define BIG_NUMBERS_SIMPLEBIGINT_H

#include <vector>
#include <string>

class SimpleBigInt {
private:
    std::vector<char> numbers;
    char sign;
    SimpleBigInt();
public:
    explicit SimpleBigInt(std::string str);
    std::string toString();
    SimpleBigInt operator+(const SimpleBigInt & another);
    SimpleBigInt operator-(SimpleBigInt another);
    bool operator>(const SimpleBigInt & another);
};

#endif //BIG_NUMBERS_SIMPLEBIGINT_H
