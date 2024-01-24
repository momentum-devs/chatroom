#pragma once

#include <memory>
#include <QList>
#include <QObject>
#include <vector>

#include "client/api/Session.h"
#include "client/gui/states/StateFactory.h"
#include "client/gui/states/StateMachine.h"
#include "client/storage/MessageStorage.h"
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
    Q_INVOKABLE void startChoosingReactions(const QString& messageId);
    Q_INVOKABLE void chooseReaction(const QString& reaction);

signals:
    void newMessageToSend(const QString& messageText);
    void messagesUpdated();
    void scrollDown();
    void getMoreMessages();

public slots:
    void handleMessageUpdate(bool shouldScrollDown = false);
    void setMessageStorage(const std::shared_ptr<storage::MessageStorage>& messageStorage);
    void requestMoreMessages();

private:
    std::shared_ptr<api::Session> session;
    const StateFactory& stateFactory;
    std::shared_ptr<StateMachine> stateMachine;
    std::shared_ptr<storage::MessageStorage> messageStorage;
    std::string currentMessageIdToChooseReaction;

    inline static const QString name{"messagesController"};
};
}