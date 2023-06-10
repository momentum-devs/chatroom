#include "SessionManager.h"

#include <gtest/gtest.h>

#include "ConnectionAcceptorMock.h"

using namespace ::testing;
using namespace server::api;

class SessionManagerTest : public Test
{
public:
    std::unique_ptr<ConnectionAcceptorMock> connectionAcceptorMockInit =
        std::make_unique<StrictMock<ConnectionAcceptorMock>>();
    ConnectionAcceptorMock* connectionAcceptorMock = connectionAcceptorMockInit.get();

    SessionManager sessionManager{std::move(connectionAcceptorMockInit)};
};

TEST_F(SessionManagerTest, startAcceptingConnections)
{
    EXPECT_CALL(*connectionAcceptorMock, startAcceptingConnections(_));

    sessionManager.startAcceptingConnections();
}
