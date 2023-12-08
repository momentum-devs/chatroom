#pragma once

#include <QObject>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class MessagesController : public QObject
{
    Q_OBJECT
public:
    MessagesController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                       std::shared_ptr<StateMachine> stateMachineInit);

    void activate();
    void deactivate();
    const QString& getName() const;

private:
    //    void handleGetUserChannelsResponse(const common::messages::Message& message);
    //    void handleGetUserDataResponse(const common::messages::Message& message);
    //    void handleGetUserChannelInvitationsResponse(const common::messages::Message& message);
    //    void handleChangeChannelInvitationResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;

    inline static const QString name{"messagesController"};
};
}