#include "MessagesController.h"

#include <ranges>

namespace client::gui
{
MessagesController::MessagesController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                       std::shared_ptr<StateMachine> stateMachineInit,
                                       std::shared_ptr<storage::MessageStorage> messageStorageInit)
    : session{std::move(sessionInit)},
      stateFactory{stateFactoryInit},
      stateMachine{std::move(stateMachineInit)},
      messageStorage{std::move(messageStorageInit)}
{
}

const QString& MessagesController::getName() const
{
    return name;
}

void MessagesController::activate() {}

void MessagesController::deactivate() {}

void MessagesController::sendMessage(const QString& text)
{
    emit newMessageToSend(text);
}

QList<QObject*> MessagesController::getMessages()
{
    QList<QObject*> result;

    for (const auto& message : messageStorage->getMessages())
    {
        result.push_back(message.get());
    }

    return result;
}

void MessagesController::handleMessageUpdate()
{
    emit messagesUpdated();
}
}