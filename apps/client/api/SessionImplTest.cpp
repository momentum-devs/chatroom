#include "SessionImpl.h"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace client::api;

namespace
{

}

class SessionImplTest : public Test
{
public:
    SessionImpl session{};
};

TEST_F(SessionImplTest, test1) {}