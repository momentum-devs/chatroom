#include "GetProjectPath.h"

#include "boost/algorithm/string/predicate.hpp"
#include "gtest/gtest.h"

#include "errors/FileNotFound.h"

using namespace ::testing;
using namespace common::filesystem;

namespace
{
const std::string validPathEnd{"chatroom"};

const std::string validProjectName{"chatroom"};
const std::string invalidProjectName{"nasjxanxjqq"};
}

TEST(GetProjectPathTest, givenValidProjectName_shouldReturnProjectPath)
{
    const auto actualProjectPath = getProjectPath(validProjectName);

    ASSERT_TRUE(boost::algorithm::ends_with(actualProjectPath, validPathEnd));
}

TEST(GetProjectPathTest, givenInvalidProjectName_shouldThrowFileNotFound)
{
    ASSERT_THROW(getProjectPath(invalidProjectName), errors::FileNotFound);
}
