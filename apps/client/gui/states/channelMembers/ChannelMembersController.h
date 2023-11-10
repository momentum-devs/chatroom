#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class ChannelMembersController : public QObject
{
    Q_OBJECT

public:
    ChannelMembersController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                             std::shared_ptr<StateMachine> stateMachine, const std::string& channelId);

    void activate();
    void deactivate();
    
    Q_INVOKABLE void goBack();
signals:

private:
    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::string channelId;
};
}