#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"
#include "client/storage/ConversationStorage.h"
#include "client/storage/MessageStorage.h"

namespace client::gui
{
class PrivateMessagesController : public QObject
{
    Q_OBJECT

public:
    PrivateMessagesController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                              std::shared_ptr<StateMachine> stateMachine,
                              std::shared_ptr<storage::ConversationStorage> conversationStorage);

    void activate();
    void deactivate();
    const QString& getName() const;

    Q_INVOKABLE void goToSendFriendInvitation();
    Q_INVOKABLE void acceptFriendInvitation(const QString& requestId);
    Q_INVOKABLE void rejectFriendInvitation(const QString& requestId);
    Q_INVOKABLE void setCurrentFriend(const QString& friendId, const QString& friendName, const QString& groupId);
    Q_INVOKABLE void removeFromFriends();

signals:
    void addFriend(const QString& friendName, const QString& friendId, const QString& groupId, bool isActive);
    void addFriendInvitation(const QString& friendName, const QString& requestId);
    void clearFriendList();
    void clearFriendInvitationList();
    void setCurrentFriendName(const QString& friendName);
    void removedFromFriends();
    void messagesUpdated(bool shouldScrollDown = false);
    void setMessageStorage(const std::shared_ptr<storage::MessageStorage>& messageStorage);

public slots:
    void goToChannel(const QString& channelName, const QString& channelId, bool isOwner);
    void sendPrivateMessage(const QString& messageText);
    void getMoreMessages();

private:
    void handleGetUserFriendsResponse(const common::messages::Message& message);
    void handleGetUserFriendInvitationsResponse(const common::messages::Message& message);
    void handleChangeFriendInvitationResponse(const common::messages::Message& message);
    void handleRemoveFromFriendsResponse();
    void handleSendPrivateMessageResponse(const common::messages::Message& message);
    void handleGetPrivateMessagesResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::shared_ptr<storage::MessageStorage> messageStorage;
    std::shared_ptr<storage::ConversationStorage> conversationStorage;
    std::string currentFriendId;
    std::string currentFriendName;
    std::string currentFriendGroupId;

    inline static const QString name{"privateMessagesController"};

    inline static const std::string getUserFriendsResponseHandlerName{
        "getUserFriendsResponseHandlerName_privateMessagesController"};
    inline static const std::string getUserFriendInvitationsResponseHandlerName{
        "getUserFriendInvitationsResponseHandlerName_privateMessagesController"};
    inline static const std::string changeFriendInvitationResponseHandlerName{
        "changeFriendInvitationResponseHandlerName_privateMessagesController"};
    inline static const std::string removeFromFriendsResponseHandlerName{
        "removeFromFriendsResponseHandlerName_privateMessagesController"};
    inline static const std::string sendPrivateMessageResponseHandlerName{
        "sendPrivateMessageResponseHandlerName_privateMessagesController"};
    inline static const std::string getPrivateMessagesResponseHandlerName{
        "getPrivateMessagesResponseHandlerName_privateMessagesController"};
};
}
