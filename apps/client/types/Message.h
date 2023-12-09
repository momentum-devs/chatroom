#pragma once

#include <memory>
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
            const QString& messageIdInit, const QDateTime& sendTimeInit, std::shared_ptr<Message> previousMessageInit);
    Message(const Message& message);

    Q_PROPERTY(bool fromUser MEMBER fromUser CONSTANT)
    Q_PROPERTY(QString messageText MEMBER messageText CONSTANT)
    Q_PROPERTY(QString senderName MEMBER senderName CONSTANT)
    Q_PROPERTY(QString messageId MEMBER messageId CONSTANT)
    Q_PROPERTY(QDateTime sendTime MEMBER sendTime CONSTANT)
    Q_PROPERTY(bool showSeparator READ shouldShowSeparator CONSTANT)

    bool shouldShowSeparator() const;

    bool operator==(const Message& rhs) const;

    Message& operator=(const Message& message);

private:
    bool fromUser;
    QString messageText;
    QString senderName;
    QString messageId;
    QDateTime sendTime;
    std::shared_ptr<Message> previousMessage;
};
}