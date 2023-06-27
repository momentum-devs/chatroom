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

signals:

private:
    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
};
}