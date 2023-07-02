#include "MessageFactoryImpl.h"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace client::api;

namespace
{

}

class MessageFactoryImplTest : public Test
{
public:
    MessageFactoryImpl messageFactory{};
};

TEST_F(MessageFactoryImplTest, test1) {}