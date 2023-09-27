#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class InviteToChannelController : public QObject
{
    Q_OBJECT

public:
    InviteToChannelController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                              std::shared_ptr<StateMachine> stateMachine, const std::string& channelId);

    void activate();
    void deactivate();

    Q_INVOKABLE void goBack();
    Q_INVOKABLE void sendChannelInvitation(const QString& email);
signals:
    void sendChannelInvitationFailure(const QString& message) const;

private:
    void handleSendChannelInvitationResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::string channelId;
    inline static const std::string sendChannelInvitationResponseHandlerName{
        "sendChannelInvitationResponseHandlerName"};
};
}