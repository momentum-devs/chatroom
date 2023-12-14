#include "Message.h"

namespace client::types
{
Message::Message(const QString& messageTextInit, const QString& senderNameInit, const QString& messageIdInit,
                 const QDateTime& sendTimeInit)
    : messageText{messageTextInit}, senderName{senderNameInit}, messageId{messageIdInit}, sendTime{sendTimeInit}
{
}

Message::Message(const Message& message)
    : QObject(),
      messageText{message.messageText},
      senderName{message.senderName},
      messageId{message.messageId},
      sendTime{message.sendTime},
      showSeparator{message.showSeparator},
      showNameAndDate{message.showNameAndDate}
{
}

bool Message::operator==(const Message& rhs) const
{
    auto tieStruct = [](const Message& message)
    {
        return std::tie(message.messageText, message.senderName, message.messageId, message.sendTime,
                        message.showSeparator, message.showNameAndDate);
    };

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
}