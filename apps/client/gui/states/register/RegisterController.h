#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class RegisterController : public QObject
{
    Q_OBJECT

public:
    RegisterController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                       std::shared_ptr<StateMachine> stateMachine);

    Q_INVOKABLE void handleRegisterRequest(const QString& email, const QString& password);
    Q_INVOKABLE void handleGoBack();

signals:
    void registerRequest(const QString& email, const QString& password);
    void goBack();

private:
    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
};
}