#include <iostream>
#include <sstream>

#include "src/big_int.hpp"
#include "src/big_float.hpp"

int main() {
//    std::string veryBigNumber1 =   "897562445645668796545246546789545467897546564879565487945656798"
//                                   "546564879545678945645754656447566546155465642145546125646541254"
//                                   "454567685345455641234566744521454576784351237684002110011000000";
//    std::string veryBigNumber2 =   "547854125415485845454548458881014011060000121153545645656987844"
//                                   "124544565016987979898989987999999999999999999999999999999999879";
//    std::string veryBigNumberSum = "897562445645668796545246546789545467897546564879565487945656799"
//                                   "094419004961164791100303115328580557215465763299091771303529098"
//                                   "579112250362443621133556732521454576784351237684002110010999879";
//
//    std::invalid_argument *invalidArgumentError = nullptr;
//    try {
//        SimpleBigInt badInt("");
//    } catch(std::invalid_argument & error) {
//        invalidArgumentError = &error;
//    }
//
//    assert(invalidArgumentError != nullptr);
//
//    invalidArgumentError = nullptr;
//    try {
//        SimpleBigInt badInt("-");
//    } catch(std::invalid_argument & error) {
//        invalidArgumentError = &error;
//    }
//
//    assert(invalidArgumentError != nullptr);
//
//    invalidArgumentError = nullptr;
//    try {
//        SimpleBigInt badInt("0001000");
//    } catch(std::invalid_argument & error) {
//        invalidArgumentError = &error;
//    }
//
//    assert(invalidArgumentError != nullptr);
//
//    invalidArgumentError = nullptr;
//    try {
//        SimpleBigInt badInt("-01000");
//    } catch(std::invalid_argument & error) {
//        invalidArgumentError = &error;
//    }
//
//    assert(invalidArgumentError != nullptr);
//
//    assert((SimpleBigInt("1") + SimpleBigInt("2")).toString() == "3");
//    assert((SimpleBigInt("10") + SimpleBigInt("1")).toString() == "11");
//    assert((SimpleBigInt("100") + SimpleBigInt("-1")).toString() == "99");
//    assert((SimpleBigInt("-1") + SimpleBigInt("100")).toString() == "99");
//    assert((SimpleBigInt("1") + SimpleBigInt("-100")).toString() == "-99");
//    assert((SimpleBigInt("-100") + SimpleBigInt("1")).toString() == "-99");
//    assert((SimpleBigInt("-100") + SimpleBigInt("100")).toString() == "0");
//    assert(SimpleBigInt("-0").toString() == "0");
//    assert((SimpleBigInt("99999999999999999999999") + SimpleBigInt("1")).toString() == "100000000000000000000000");
//    assert((SimpleBigInt("99999999999999999999999") + SimpleBigInt("123465795498789987")).toString() == "100000123465795498789986");
//    assert((SimpleBigInt(veryBigNumber1) + SimpleBigInt(veryBigNumber2)).toString() == veryBigNumberSum);

//    big_int first({0b10000000, 0b00000001}, 0);
//    big_int second({0b11111111}, 0);
//
//    big_int b = parse_big_int<uint8_t>("-54657985435165798");

//    std::cout << (-first + second).binary_str() << std::endl;

    big_int<uint8_t> val = (big_int<uint8_t>) (char) -127;

    std::cout << val.binary_str() << std::endl;
    std::cout << (val << 2).binary_str() << std::endl;

    std::cout << "All tests completed successfully" << std::endl;

    return 0;
}
