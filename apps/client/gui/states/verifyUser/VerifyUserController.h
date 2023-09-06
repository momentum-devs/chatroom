#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class VerifyUserController : public QObject
{
    Q_OBJECT

public:
    VerifyUserController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                         std::shared_ptr<StateMachine> stateMachine);

    void activate();
    void deactivate();

    Q_INVOKABLE void verificationRequest(const QString& verificationCode);
    Q_INVOKABLE void goToLoginState();

signals:
    void verificationFailure(const QString& error);

private:
    void handleVerificationResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    inline const static std::string verificationResponseHandlerName{"verificationResponseHandlerName"};
};
}