#include "MessageHandlerImpl.h"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace server::api;

namespace
{

}

class MessageHandlerImplTest : public Test
{
public:
    MessageHandlerImpl messageHandler{};
};

TEST_F(MessageHandlerImplTest, test1) {}