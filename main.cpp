#include <iostream>

#include "bigint.h"

int main() {
    BigInt<uint8_t> a({ 255, 5 });
    BigInt<uint8_t> b({ 255 });

    BigInt<uint8_t> c = a + b;

    return 0;
}
