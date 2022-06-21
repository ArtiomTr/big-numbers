#include <fstream>

#include "ParsingUtils.h"
#include "IsomorphicMath.h"
#include "BigInt.h"
#include "../utils.h"

using namespace BigNumbers;

int main() {
    std::ifstream input = safeRelativeOpen("factorial.txt");

    bool failure = false;
    while (!input.eof() && !failure) {
        int in;
        BigInt result;

        input >> in >> result;

        if (IsomorphicMath::factorial<BigInt>(in) != result) {
            std::cout << "Failed to compute " << in << "!" << std::endl;
            failure = true;
        }
    }

    input.close();

    std::cout << "All factorials computed successfully!" << std::endl;

    return failure ? 1 : 0;
}