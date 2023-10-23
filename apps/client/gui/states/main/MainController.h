#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class MainController : public QObject
{
    Q_OBJECT

public:
    MainController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                   std::shared_ptr<StateMachine> stateMachine);

    void activate();
    void deactivate();
    Q_INVOKABLE void logout();
    Q_INVOKABLE void goToCreateChannel();
    Q_INVOKABLE void goToSendFriendRequest();
    Q_INVOKABLE void goToUserSettings();
    Q_INVOKABLE void setCurrentChannel(const QString& channelId);
    Q_INVOKABLE void addToChannel();
    Q_INVOKABLE void leftTheChannel();
    Q_INVOKABLE void deleteTheChannel();
    Q_INVOKABLE void acceptChannelInvitation(const QString& channelId);
    Q_INVOKABLE void rejectChannelInvitation(const QString& channelId);
    Q_INVOKABLE void acceptFriendRequest(const QString& channelId);
    Q_INVOKABLE void rejectFriendRequest(const QString& channelId);

signals:
    void addChannel(const QString& channelName, const QString& channelId, bool isOwner);
    void addChannelInvitation(const QString& channelName, const QString& channelId);
    void clearChannelList();
    void clearChannelInvitationList();
    void addFriend(const QString& friendName, const QString& friendId);
    void addFriendRequest(const QString& friendName, const QString& requestId);
    void clearFriendList();
    void clearFriendRequestList();

private:
    void handleGetUserChannelsResponse(const common::messages::Message& message);
    void handleGetUserDataResponse(const common::messages::Message& message);
    void handleLeftTheChannelResponse(const common::messages::Message& message);
    void handleGetUserChannelInvitationsResponse(const common::messages::Message& message);
    void handleChangeChannelInvitationResponse(const common::messages::Message& message);
    void handleGetUserFriendsResponse(const common::messages::Message& message);
    void handleGetUserFriendRequestsResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::optional<std::shared_ptr<State>> nextState = std::nullopt;
    std::string currentChannelId;

    inline static const std::string getUserChannelsResponseHandlerName{"getUserChannelsResponseHandlerName"};
    inline static const std::string getUserDataResponseHandlerName{"getUserChannelsResponseHandlerName"};
    inline static const std::string leftTheChannelResponseHandlerName{"leftTheChannelResponseHandlerName"};
    inline static const std::string deleteTheChannelResponseHandlerName{"deleteTheChannelResponseHandlerName"};
    inline static const std::string changeChannelInvitationResponseHandlerName{
        "changeChannelInvitationResponseHandlerName"};
    inline static const std::string getUserChannelInvitationsResponseHandlerName{
        "getUserChannelInvitationsResponseHandlerName"};
    inline static const std::string getUserFriendsResponseHandlerName{"getUserFriendsResponseHandlerName"};
    inline static const std::string getUserFriendRequestsResponseHandlerName{
        "getUserFriendRequestsResponseHandlerName"};
};
}