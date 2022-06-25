#include "BigFloatMath.h"

#include <sstream>

#include "IsomorphicMath.h"

namespace BigNumbers {
    BigInt ceil(const BigFloat &value) {
        std::stringstream builder;
        builder << value;
        std::string valueStr = builder.str();
        auto position = valueStr.find('.');
        BigInt castedValue;
        builder.str(valueStr.substr(0, position));
        builder >> castedValue;

        if (position != std::string::npos && castedValue > 0) {
            castedValue += 1;
        }

        return castedValue;
    }

    BigFloat sin(BigFloat value) {
        constexpr int ITERATION_COUNT = 40;

        BigFloat currentPi = IsomorphicMath::pi(BigFloat::epsilon(value.getPrecision()), value.getDecimalPrecision());
        BigFloat pi2 = currentPi / 2;
        BigFloat mPi2 = -pi2;

        bool negativeMultiplier = false;

        if (value > pi2 || value < mPi2) {
            BigFloat count = value / currentPi;
            BigInt v = ceil(count);
            value -= (BigFloat) v * currentPi;
            negativeMultiplier = static_cast<int8_t>(v % BigNumbers::BigInt(2));
        }

        BigFloat computedSine = value;
        BigFloat currentValue = value;
        BigFloat currentFactorial = 1;

        for (int i = 1; i < ITERATION_COUNT; ++i) {
            currentValue *= value;
            currentValue *= value;
            currentFactorial *= static_cast<BigFloat>(2 * i * (2 * i + 1));

            if (i % 2 == 0) {
                computedSine += (currentValue / currentFactorial);
            } else {
                computedSine -= (currentValue / currentFactorial);
            }
        }

        if (negativeMultiplier) {
            computedSine = -computedSine;
        }

        return computedSine;
    }
}