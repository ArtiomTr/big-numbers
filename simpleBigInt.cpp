#include "simpleBigInt.h"

#include <stdexcept>

const std::string EINVALID_BIGINT = "Expected a string representing big integer";

SimpleBigInt::SimpleBigInt(): sign(1) {}

SimpleBigInt::SimpleBigInt(std::string str) {
    if(str.length() == 0 || (str.length() == 1 && str[0] == '-')) {
        throw std::invalid_argument(EINVALID_BIGINT);
    }

    int beginIndex = 0;
    if(str[0] == '-') {
        sign = -1;
        beginIndex = 1;
    } else {
        sign = 1;
    }

    if(str[beginIndex] == '0' && str.size() - beginIndex > 1) {
        throw std::invalid_argument(EINVALID_BIGINT);
    }

    for(long long int i = static_cast<long long int>(str.length()) - 1; i >= beginIndex; --i) {
        if(!std::isdigit(str[i])) {
            throw std::invalid_argument(EINVALID_BIGINT);
        }
        numbers.push_back(static_cast<char>(str[i] - '0'));
    }
}

SimpleBigInt SimpleBigInt::operator+(const SimpleBigInt & another) {
    if(sign == -1 || another.sign == -1) {
        SimpleBigInt a = *this, b = another;

        b.sign *= -1;

        return a - b;
    }

    SimpleBigInt output{};

    char carry = 0;
    std::vector<char> longestVector = numbers.size() > another.numbers.size() ? numbers : another.numbers;
    std::vector<char>::size_type minSize = std::min(numbers.size(), another.numbers.size());
    for(std::vector<char>::size_type i = 0; i < longestVector.size(); ++i) {
        char sum;

        if(i >= minSize) {
            sum = static_cast<char>(longestVector[i] + carry);
        } else {
            sum = static_cast<char>(numbers[i] + another.numbers[i] + carry);
        }

        if(sum >= 10) {
            sum %= 10;
            carry = 1;
        } else {
            carry = 0;
        }

        output.numbers.push_back(sum);
    }

    if(carry > 0) {
        output.numbers.push_back(carry);
    }

    return output;
}

SimpleBigInt SimpleBigInt::operator-(SimpleBigInt another) {
    if(sign == 1 && another.sign == -1) {
        another.sign = 1;
        return *this + another;
    }

    if(sign == -1 && another.sign == 1) {
        SimpleBigInt a = another, b = *this;
        b.sign = 1;
        SimpleBigInt out = a + b;
        out.sign = -1;

        return out;
    }

    SimpleBigInt minuend{}, subtrahend{};

    if(sign == -1) {
        minuend = another;
        subtrahend = *this;
        subtrahend.sign = 1;
        minuend.sign = 1;
    } else {
        minuend = *this;
        subtrahend = another;
    }

    SimpleBigInt output{};
    if(minuend > subtrahend) {
        output.sign = 1;
    } else {
        std::swap(minuend, subtrahend);
        output.sign = -1;
    }

    char carry = 0;
    std::vector<char>::size_type subtrahendNumbersLength = subtrahend.numbers.size();
    for(std::vector<char>::size_type i = 0; i < minuend.numbers.size(); ++i) {
        char current = static_cast<char>(minuend.numbers[i] - carry);
        if(i < subtrahendNumbersLength) {
            current = static_cast<char>(current - subtrahend.numbers[i]);
        }

        if(current < 0) {
            carry = 1;
            current = (char) (10 + current);
        } else {
            carry = 0;
        }

        output.numbers.push_back(current);
    }

    while(output.numbers[output.numbers.size() - 1] == 0 && output.numbers.size() > 1) {
        output.numbers.erase(output.numbers.end() - 1);
    }

    return output;
}

bool SimpleBigInt::operator>(const SimpleBigInt & another) {
    if(sign != another.sign) {
        return sign > another.sign;
    }

    if(numbers.size() != another.numbers.size()) {
        return numbers.size() > another.numbers.size();
    }

    for(int i = 0; i < numbers.size(); ++i) {
        if(numbers[i] > another.numbers[i]) {
            return true;
        }
    }

    return false;
}

std::string SimpleBigInt::toString() {
    std::string str;

    for(char number : numbers) {
        str += static_cast<char>(number + '0');
    }

    std::string output(str.rbegin(), str.rend());

    if(sign == -1 && output != "0") {
        output = "-" + output;
    }

    return output;
}