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
                      std::shared_ptr<StateMachine> stateMachine, const std::string& initialChannelId,
                      const std::string& initialChannelName, bool initialIsChannelOwner);

    void activate();
    void deactivate();
    const QString& getName() const;

    Q_INVOKABLE void addToChannel();
    Q_INVOKABLE void leaveChannel();
    Q_INVOKABLE void deleteChannel();

signals:
    void setChannel(const QString& channelName, const QString& channelId, bool isOwner);

public slots:
    void goToChannel(const QString& channelName, const QString& channelId, bool isOwner);
    void goToPrivateMessages();

private:
    void handleLeftChannelResponse(const common::messages::Message& message);

    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::string currentChannelId;
    std::string currentChannelName;
    bool isOwnerOfCurrentChannel;

    inline static const QString name{"channelController"};

    inline static const std::string leftChannelResponseHandlerName{"leftChannelResponseHandlerName"};
    inline static const std::string deleteChannelResponseHandlerName{"deleteChannelResponseHandlerName"};
};
}