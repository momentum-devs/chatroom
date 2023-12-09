#pragma once

#include <memory>
#include <QList>
#include <QObject>
#include <vector>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"
#include "client/types/Message.h"

namespace client::gui
{
class MessagesController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> messages READ getMessages NOTIFY messagesUpdated)
public:
    MessagesController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                       std::shared_ptr<StateMachine> stateMachineInit);

    void activate();
    void deactivate();
    const QString& getName() const;

    QList<QObject*> getMessages();

    Q_INVOKABLE void sendMessage(const QString& text);

signals:
    void newMessageToSend(const types::Message& message);
    void messagesUpdated();

private:
    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;

    inline static const QString name{"messagesController"};

    std::vector<std::shared_ptr<types::Message>> messages;
};
}