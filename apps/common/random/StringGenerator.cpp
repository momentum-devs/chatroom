#include "StringGenerator.h"

#include "Helper.h"

namespace common::random
{

std::string common::random::StringGenerator::numeric(unsigned int length)
{
    const std::string numericCharactersWithoutZero = "123456789";
    const std::string numericCharacters = "0123456789";

    std::string numeric;

    for (unsigned i = 0; i < length; i++)
    {
        if (i == 0)
        {
            numeric += Helper::arrayElement<char>(numericCharactersWithoutZero);
        }
        else
        {
            numeric += Helper::arrayElement<char>(numericCharacters);
        }
    }

    return numeric;
}
}
