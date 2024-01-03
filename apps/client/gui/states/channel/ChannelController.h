#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"
#include "client/storage/ConversationStorage.h"
#include "client/storage/MessageStorage.h"
#include "client/types/Message.h"

namespace client::gui
{
class ChannelController : public QObject
{
    Q_OBJECT

public:
    ChannelController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                      std::shared_ptr<StateMachine> stateMachine,
                      std::shared_ptr<storage::ConversationStorage> conversationStorage,
                      const std::string& initialChannelId, const std::string& initialChannelName,
                      bool initialIsChannelOwner);

    void activate();
    void deactivate();
    const QString& getName() const;

    Q_INVOKABLE void addToChannel();
    Q_INVOKABLE void leaveChannel();
    Q_INVOKABLE void deleteChannel();

signals:
    void setChannel(const QString& channelName, const QString& channelId, bool isOwner);
    void addMember(const QString& memberName, const QString& memberId, bool isActive);
    void clearMembersList();
    void messagesUpdated(bool shouldScrollDown = false);
    void setMessageStorage(const std::shared_ptr<storage::MessageStorage>& messageStorage);

public slots:
    void goToChannel(const QString& channelName, const QString& channelId, bool isOwner);
    void goToPrivateMessages();
    void sendChannelMessage(const QString& messageText);
    void getMoreMessages();

private:
    void handleLeftChannelResponse(const common::messages::Message& message);
    void handleGetChannelMembersResponse(const common::messages::Message& message);
    void handleSendChannelMessageResponse(const common::messages::Message& message);
    void handleGetChannelMessagesResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::shared_ptr<storage::MessageStorage> messageStorage;
    std::shared_ptr<storage::ConversationStorage> conversationStorage;
    std::string currentChannelId;
    std::string currentChannelName;
    bool isOwnerOfCurrentChannel;
    bool isActivated;

    inline static const QString name{"channelController"};

    inline static const std::string leftChannelResponseHandlerName{"leftChannelResponseHandlerName"};
    inline static const std::string deleteChannelResponseHandlerName{"deleteChannelResponseHandlerName"};
    inline static const std::string getChannelMembersResponseHandlerName{"getChannelMembersResponseHandlerName"};
    inline static const std::string sendChannelMessageResponseHandlerName{"sendChannelMessageResponseHandlerName"};
    inline static const std::string getChannelMessagesResponseHandlerName{"getChannelMessagesResponseHandlerName"};
};
}