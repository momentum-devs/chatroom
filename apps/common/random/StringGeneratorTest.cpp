#include "StringGenerator.h"

#include <algorithm>

#include "gtest/gtest.h"

using namespace ::testing;
using namespace common::random;

class StringGeneratorTest : public Test
{
public:
};

TEST_F(StringGeneratorTest, shouldGenerateNumeric)
{
    const std::string numericCharactersWithoutZero = "123456789";
    const std::string numericCharacters = "0123456789";

    const unsigned long numericLength = 8;

    const auto numeric = StringGenerator::numeric(numericLength);

    const auto nonZeroCharacter = numeric[0];
    const auto numericWithPossibleZeroCharacters = numeric.substr(1);

    ASSERT_EQ(numeric.size(), numericLength);
    ASSERT_TRUE(std::ranges::any_of(numericCharactersWithoutZero, [nonZeroCharacter](char numericCharacter)
                                    { return nonZeroCharacter == numericCharacter; }));
    ASSERT_TRUE(std::ranges::all_of(numericWithPossibleZeroCharacters,
                                    [numericCharacters](char numericCharacterWithPossibleZero)
                                    {
                                        return std::ranges::any_of(
                                            numericCharacters, [numericCharacterWithPossibleZero](char numericCharacter)
                                            { return numericCharacterWithPossibleZero == numericCharacter; });
                                    }));
}
