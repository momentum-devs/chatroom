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

    void activate();
    void deactivate();
    Q_INVOKABLE void registerRequest(const QString& email, const QString& password);
    Q_INVOKABLE void goBack();

signals:
    void registerFailure(const QString& message);

private:
    void handleRegisterResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    inline static const std::string registerResponseHandlerName{"registerResponseHandlerName"};
};
}