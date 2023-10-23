#include "NumberGenerator.h"

#include <algorithm>

#include "gtest/gtest.h"

using namespace ::testing;
using namespace common::random;

class NumberGeneratorTest : public Test
{
public:
};

TEST_F(NumberGeneratorTest, integer_givenInvalidRangeArguments_shouldThrowInvalidArgument)
{
    ASSERT_THROW(NumberGenerator::integer<int>(10, 2), std::invalid_argument);
}

TEST_F(NumberGeneratorTest, givenRangeWithSameNumbers_shouldGenerateThisNumber)
{
    const int actualRandomNumber = NumberGenerator::integer<int>(2, 2);

    ASSERT_EQ(actualRandomNumber, 2);
}

TEST_F(NumberGeneratorTest, givenValidRange_shouldGenerateNumberWithinGivenRange)
{
    const int actualRandomNumber = NumberGenerator::integer<int>(2, 10);

    ASSERT_TRUE(actualRandomNumber >= 2);
    ASSERT_TRUE(actualRandomNumber <= 10);
}

TEST_F(NumberGeneratorTest, givenSingleArgument_shouldCorrectlyResolveToTwoArgsOverload)
{
    const int randomNumber = NumberGenerator::integer<int>(10);

    ASSERT_TRUE(randomNumber >= 0);
    ASSERT_TRUE(randomNumber <= 10);
}
