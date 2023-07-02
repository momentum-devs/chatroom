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

    void activate();
    void deactivate();
    Q_INVOKABLE void loginRequest(const QString& email, const QString& password);
    Q_INVOKABLE void goToRegisterState();

signals:
    void loginFailure(const QString& message);

private:
    void handleLoginResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    inline const static std::string loginResponseHandlerName{"loginResponseHandlerName"};
};
}