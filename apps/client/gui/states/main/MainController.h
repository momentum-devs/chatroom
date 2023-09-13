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
    Q_INVOKABLE void logout();
    Q_INVOKABLE void goToCreateChannel();
    Q_INVOKABLE void goToSendFriendRequest();
    Q_INVOKABLE void goToUserSettings();

signals:
    void addChannel(const QString& channelName, const QString& channelId);

private:
    void handleGetUserChannelsResponse(const common::messages::Message& message);
    void handleGetUserDataResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::optional<std::shared_ptr<State>> nextState = std::nullopt;

    inline static const std::string getUserChannelsResponseHandlerName{"getUserChannelsResponseHandlerName"};
    inline static const std::string getUserDataResponseHandlerName{"getUserChannelsResponseHandlerName"};
};
}