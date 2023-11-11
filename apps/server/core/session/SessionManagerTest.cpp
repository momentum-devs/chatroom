#include "SessionManager.h"

#include <gtest/gtest.h>

#include "ConnectionAcceptorMock.h"
#include "SessionMock.h"

using namespace ::testing;
using namespace server::core;

class SessionManagerTest : public Test
{
public:
    std::shared_ptr<SessionMock> activeSessionMock = std::make_shared<StrictMock<SessionMock>>();

    std::shared_ptr<SessionMock> inactiveSessionMock = std::make_shared<StrictMock<SessionMock>>();

    std::unique_ptr<ConnectionAcceptorMock> connectionAcceptorMockInit =
        std::make_unique<StrictMock<ConnectionAcceptorMock>>();
    ConnectionAcceptorMock* connectionAcceptorMock = connectionAcceptorMockInit.get();

    SessionManager sessionManager{std::move(connectionAcceptorMockInit)};

    std::function<void(const std::shared_ptr<Session>& session)> handleNewConnection;

    void startAcceptingConnections()
    {
        EXPECT_CALL(*connectionAcceptorMock, startAcceptingConnections(_)).WillOnce(SaveArg<0>(&handleNewConnection));

        sessionManager.startAcceptingConnections();
    }
};

TEST_F(SessionManagerTest, startAcceptingConnections)
{
    EXPECT_CALL(*connectionAcceptorMock, startAcceptingConnections(_));

    sessionManager.startAcceptingConnections();
}

TEST_F(SessionManagerTest, removeInactiveSessions)
{
    startAcceptingConnections();

    handleNewConnection(activeSessionMock);
    handleNewConnection(inactiveSessionMock);

    EXPECT_CALL(*activeSessionMock, isActive()).WillOnce(Return(true));
    EXPECT_CALL(*inactiveSessionMock, isActive()).WillOnce(Return(false));
    EXPECT_CALL(*inactiveSessionMock, close());

    sessionManager.removeInactiveSessions();
}
