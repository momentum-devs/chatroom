#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class LoginController : public QObject
{
    Q_OBJECT

public:
    LoginController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                    std::shared_ptr<StateMachine> stateMachine);

    Q_INVOKABLE void handleLoginRequest(const QString& email, const QString& password);
    Q_INVOKABLE void handleGoToRegisterState();

signals:
    void loginRequest(const QString& email, const QString& password);
    void goToRegisterState();

private:
    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
};
}