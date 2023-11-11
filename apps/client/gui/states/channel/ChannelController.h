#pragma once

#include <memory>
#include <QObject>
#include <QString>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"

namespace client::gui
{
class ChannelController : public QObject
{
    Q_OBJECT

public:
    ChannelController(std::shared_ptr<api::Session> session, const StateFactory& stateFactory,
                      std::shared_ptr<StateMachine> stateMachine, const std::string& initialChannelId);

    void activate();
    void deactivate();
    const QString& getName() const;

public slots:
    void goToChannel(const QString& channelId);
    void goToPrivateMessages();

private:
    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::string currentChannelId;

    inline static const QString name{"channelController"};
};
}