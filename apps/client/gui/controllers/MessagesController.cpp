#include "MessagesController.h"

#include <cstdlib>

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
    auto previousMessage = messages.empty() ? nullptr : messages.back();

    auto date = QDateTime::currentDateTimeUtc();

    auto userName = dynamic_cast<types::User*>(session->getUser())->property("nickname").toString();

    auto message = std::make_shared<types::Message>(false, text, userName, "", date, previousMessage);

    messages.push_back(message);

    emit messagesUpdated();

    emit newMessageToSend(*message);
}

QList<QObject*> MessagesController::getMessages()
{
    QList<QObject*> result;

    for (auto& message : messages)
    {
        result.push_back(dynamic_cast<QObject*>(message.get()));
    }

    return result;
}
}