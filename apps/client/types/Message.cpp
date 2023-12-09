#include "Message.h"

namespace client::types
{
Message::Message(bool fromUserInit, const QString& messageTextInit, const QString& senderNameInit,
                 const QString& messageIdInit, const QDateTime& sendTimeInit, Message* previousMessageInit)
    : fromUser{fromUserInit},
      messageText{messageTextInit},
      senderName{senderNameInit},
      messageId{messageIdInit},
      sendTime{sendTimeInit},
      previousMessage{previousMessageInit}
{
}

Message::Message(const Message& message)
    : QObject(),
      fromUser{message.fromUser},
      messageText{message.messageText},
      senderName{message.senderName},
      messageId{message.messageId},
      sendTime{message.sendTime}
{
}

bool Message::operator==(const Message& rhs) const
{
    auto tieStruct = [](const Message& message) {
        return std::tie(message.fromUser, message.messageText, message.senderName, message.messageId, message.sendTime);
    };

    return tieStruct(*this) == tieStruct(rhs);
}

Message& Message::operator=(const Message& message)
{
    fromUser = message.fromUser;
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
}