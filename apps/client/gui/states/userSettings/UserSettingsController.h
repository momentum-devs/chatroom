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
    void updateUserFailure(const QString& error);
    void deleteUserFailure(const QString& error);
    void setUserData(const QString& email, const QString& nick);

private:
    void handleUpdateUserResponse(const common::messages::Message& message);
    void handleDeleteUserResponse(const common::messages::Message& message);
    void handleGetUserDataResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    inline static const std::string updateUserResponseHandlerName{"updateUserResponseHandlerName"};
    inline static const std::string deleteUserResponseHandlerName{"deleteUserResponseHandlerName"};
    inline static const std::string getUserDataResponseHandlerName{"getUserDataResponseHandlerName"};
};
}