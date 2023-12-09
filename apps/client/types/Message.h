#pragma once

#include <QDateTime>
#include <QList>
#include <QObject>

namespace client::types
{
class Message : public QObject
{
    Q_OBJECT

public:
    Message(bool fromUserInit, const QString& messageTextInit, const QString& senderNameInit,
            const QString& messageIdInit, const QDateTime& sendTimeInit, Message* previousMessageInit);
    Message(const Message& message);

    bool fromUser;
    QString messageText;
    QString senderName;
    QString messageId;
    QDateTime sendTime;
    Message* previousMessage;

    Q_PROPERTY(bool fromUser MEMBER fromUser CONSTANT)
    Q_PROPERTY(QString messageText MEMBER messageText CONSTANT)
    Q_PROPERTY(QString senderName MEMBER senderName CONSTANT)
    Q_PROPERTY(QString messageId MEMBER messageId CONSTANT)
    Q_PROPERTY(QDateTime sendTime MEMBER sendTime CONSTANT)
    Q_PROPERTY(QObject* previousMessage MEMBER previousMessage CONSTANT)
    Q_PROPERTY(bool showSeparator READ shouldShowSeparator CONSTANT)

    bool shouldShowSeparator() const;

    bool operator==(const Message& rhs) const;

    Message& operator=(const Message& message);
};
}