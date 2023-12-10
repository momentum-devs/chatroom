#include "Message.h"

namespace client::types
{
Message::Message(const QString& messageTextInit, const QString& senderNameInit, const QString& messageIdInit,
                 const QDateTime& sendTimeInit, std::shared_ptr<Message> previousMessageInit)
    : messageText{messageTextInit},
      senderName{senderNameInit},
      messageId{messageIdInit},
      sendTime{sendTimeInit},
      previousMessage{std::move(previousMessageInit)}
{
}

Message::Message(const Message& message)
    : QObject(),
      messageText{message.messageText},
      senderName{message.senderName},
      messageId{message.messageId},
      sendTime{message.sendTime}
{
}

bool Message::operator==(const Message& rhs) const
{
    auto tieStruct = [](const Message& message)
    { return std::tie(message.messageText, message.senderName, message.messageId, message.sendTime); };

    return tieStruct(*this) == tieStruct(rhs);
}

Message& Message::operator=(const Message& message)
{
    messageText = message.messageText;
    senderName = message.senderName;
    messageId = message.messageId;
    sendTime = message.sendTime;
    return *this;
}

bool Message::shouldShowSeparator() const
{
    return previousMessage != nullptr and previousMessage->sendTime.date() != sendTime.date();
}

bool Message::shouldShowNameAndDate() const
{
    return previousMessage == nullptr or previousMessage->senderName != senderName or
           previousMessage->sendTime.addSecs(5 * 60) < sendTime;
}
}