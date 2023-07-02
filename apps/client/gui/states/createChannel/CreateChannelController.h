#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class CreateChannelController : public QObject
{
    Q_OBJECT

public:
    CreateChannelController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                            std::shared_ptr<StateMachine> stateMachine);

    void activate();
    void deactivate();

    Q_INVOKABLE void goBack();
    Q_INVOKABLE void createChannel(const QString& channelName);
signals:

private:
    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
};
}