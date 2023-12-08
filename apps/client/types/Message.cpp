#include "Message.h"

namespace client::types
{
Message::Message(bool fromUserInit, const QString& messageTextInit, const QString& senderIdInit,
                 const QString& messageIdInit, const QString& sendTimeInit)
    : fromUser{fromUserInit},
      messageText{messageTextInit},
      senderId{senderIdInit},
      messageId{messageIdInit},
      sendTime{sendTimeInit}
{
}

Message::Message(const Message& message)
    : QObject(),
      fromUser{message.fromUser},
      messageText{message.messageText},
      senderId{message.senderId},
      messageId{message.messageId},
      sendTime{message.sendTime}
{
}

bool Message::operator==(const Message& rhs) const
{
    auto tieStruct = [](const Message& message)
    { return std::tie(message.fromUser, message.messageText, message.senderId, message.messageId, message.sendTime); };

    return tieStruct(*this) == tieStruct(rhs);
}

Message& Message::operator=(const Message& message)
{
    fromUser = message.fromUser;
    messageText = message.messageText;
    senderId = message.senderId;
    messageId = message.messageId;
    sendTime = message.sendTime;
    return *this;
}
}