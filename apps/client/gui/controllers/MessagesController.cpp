#include "MessagesController.h"

namespace client::gui
{
MessagesController::MessagesController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                       std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
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
    messages.emplace_back(true, text, "", "", "time");

    emit messagesUpdated();
}

QList<QObject*> MessagesController::getMessages()
{
    QList<QObject*> result;

    for (auto& message : messages)
    {
        result.push_back(dynamic_cast<QObject*>(&message));
    }
    
    return result;
}
}