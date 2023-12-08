#pragma once

#include <QList>
#include <QObject>

namespace client::types
{
class Message : public QObject
{
    Q_OBJECT
    
public:
    Message(bool fromUserInit, const QString& messageTextInit, const QString& senderIdInit,
            const QString& messageIdInit, const QString& sendTimeInit);
    Message(const Message& message);

    bool fromUser;
    QString messageText;
    QString senderId;
    QString messageId;
    QString sendTime;

    Q_PROPERTY(bool fromUser MEMBER fromUser)
    Q_PROPERTY(QString messageText MEMBER messageText)
    Q_PROPERTY(QString senderId MEMBER senderId)
    Q_PROPERTY(QString messageId MEMBER messageId)
    Q_PROPERTY(QString sendTime MEMBER sendTime)

    bool operator==(const Message& rhs) const;

    Message& operator=(const Message& message);
};
}