#include "MessagesController.h"

#include <loguru.hpp>
#include <ranges>

namespace client::gui
{
MessagesController::MessagesController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                       std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)},
      stateFactory{stateFactoryInit},
      stateMachine{std::move(stateMachineInit)},
      messageStorage{std::make_shared<storage::MessageStorage>()}
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

void MessagesController::handleMessageUpdate(bool shouldScrollDown)
{
    emit messagesUpdated();

    if (shouldScrollDown)
    {
        emit scrollDown();
    }
}

void MessagesController::setMessageStorage(const std::shared_ptr<storage::MessageStorage>& messageStorage)
{
    this->messageStorage = messageStorage;

    emit messagesUpdated();

    emit scrollDown();
}

void MessagesController::requestMoreMessages()
{
    LOG_S(INFO) << "Requesting more messages";

    emit getMoreMessages();
}
}