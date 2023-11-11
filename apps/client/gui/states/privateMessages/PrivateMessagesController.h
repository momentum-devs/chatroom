#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class PrivateMessagesController : public QObject
{
    Q_OBJECT

public:
    PrivateMessagesController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                              std::shared_ptr<StateMachine> stateMachine);

    void activate();
    void deactivate();
    const QString& getName() const;

    Q_INVOKABLE void goToSendFriendRequest();
    Q_INVOKABLE void acceptFriendRequest(const QString& requestId);
    Q_INVOKABLE void rejectFriendRequest(const QString& requestId);
    Q_INVOKABLE void setCurrentFriend(const QString& friendId, const QString& friendName);
    Q_INVOKABLE void removeFromFriends();

signals:
    void addFriend(const QString& friendName, const QString& friendId, bool isActive);
    void addFriendRequest(const QString& friendName, const QString& requestId);
    void clearFriendList();
    void clearFriendRequestList();
    void setCurrentFriendName(const QString& friendNam);
    void removedFromFriends();

public slots:
    void goToChannel(const QString& channelId);

private:
    void handleGetUserFriendsResponse(const common::messages::Message& message);
    void handleGetUserFriendRequestsResponse(const common::messages::Message& message);
    void handleChangeFriendRequestResponse(const common::messages::Message& message);
    void handleRemoveFromFriendsResponse();

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::string currentFriendId;

    inline static const QString name{"privateMessagesController"};

    inline static const std::string getUserFriendsResponseHandlerName{
        "getUserFriendsResponseHandlerName_privateMessagesController"};
    inline static const std::string getUserFriendRequestsResponseHandlerName{
        "getUserFriendRequestsResponseHandlerName_privateMessagesController"};
    inline static const std::string changeFriendRequestResponseHandlerName{
        "changeFriendRequestResponseHandlerName_privateMessagesController"};
    inline static const std::string removeFromFriendsResponseHandlerName{
        "removeFromFriendsResponseHandlerName_privateMessagesController"};
};
}