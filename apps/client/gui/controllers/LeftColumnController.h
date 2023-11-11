#pragma once

#include <QObject>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class LeftColumnController : public QObject
{
    Q_OBJECT
public:
    LeftColumnController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                         std::shared_ptr<StateMachine> stateMachineInit);

    void activate();
    void deactivate();
    const QString& getName() const;

    Q_INVOKABLE void goToPrivateMessages();
    Q_INVOKABLE void goToCreateChannel();
    Q_INVOKABLE void goToUserSettings();
    Q_INVOKABLE void goToChannel(const QString& channelId);

signals:
    void addChannel(const QString& channelName, const QString& channelId, bool isOwner);
    void addChannelInvitation(const QString& channelName, const QString& channelId);
    void clearChannelList();
    void clearChannelInvitationList();
    void goToChannelSignal(const QString& channelId);
    void setUserName(const QString& userName);

private:
    void handleGetUserChannelsResponse(const common::messages::Message& message);
    void handleGetUserDataResponse(const common::messages::Message& message);
    void handleGetUserChannelInvitationsResponse(const common::messages::Message& message);
    void handleChangeChannelInvitationResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;

    inline static const QString name{"leftColumnController"};

    inline static const std::string getUserChannelsResponseHandlerName{
        "getUserChannelsResponseHandlerName_LeftColumnController"};
    inline static const std::string getUserDataResponseHandlerName{
        "getUserChannelsResponseHandlerName_LeftColumnController"};
    inline static const std::string changeChannelInvitationResponseHandlerName{
        "changeChannelInvitationResponseHandlerName_LeftColumnController"};
    inline static const std::string getUserChannelInvitationsResponseHandlerName{
        "getUserChannelInvitationsResponseHandlerName_LeftColumnController"};
};
}