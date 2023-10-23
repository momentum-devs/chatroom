#pragma once

#include <concepts>
#include <random>
#include <stdexcept>
#include <type_traits>

namespace common::random
{
class NumberGenerator
{
public:
    template <std::integral I>
    static I integer(I min, I max)
    {
        if (min > max)
        {
            throw std::invalid_argument("Minimum value must be smaller than maximum value.");
        }

        std::uniform_int_distribution<I> distribution(min, max);

        return distribution(pseudoRandomGenerator);
    }

    template <std::integral I>
    static I integer(I max)
    {
        return NumberGenerator::integer<I>(static_cast<I>(0), max);
    }

private:
    static std::random_device randomDevice;
    static std::mt19937 pseudoRandomGenerator;
};
}
