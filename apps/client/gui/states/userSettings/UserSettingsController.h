#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class UserSettingsController : public QObject
{
    Q_OBJECT

public:
    UserSettingsController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                           std::shared_ptr<StateMachine> stateMachine);

    void activate();
    void deactivate();

    Q_INVOKABLE void changeNickname(const QString& nickname);
    Q_INVOKABLE void changePassword(const QString& password);
    Q_INVOKABLE void deleteUser();
    Q_INVOKABLE void goBack();
signals:
    void changeNicknameFailure(const QString& error);
    void changePasswordFailure(const QString& error);

private:
    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
};
}