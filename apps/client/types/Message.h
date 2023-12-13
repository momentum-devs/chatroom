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
    Message(const QString& messageTextInit, const QString& senderNameInit, const QString& messageIdInit,
            const QDateTime& sendTimeInit, std::shared_ptr<Message> previousMessageInit);
    Message(const Message& message);

    Q_PROPERTY(QString messageText MEMBER messageText CONSTANT)
    Q_PROPERTY(QString senderName MEMBER senderName CONSTANT)
    Q_PROPERTY(QString messageId MEMBER messageId CONSTANT)
    Q_PROPERTY(QDateTime sendTime MEMBER sendTime CONSTANT)
    Q_PROPERTY(bool showSeparator READ shouldShowSeparator CONSTANT)
    Q_PROPERTY(bool showNameAndDate READ shouldShowNameAndDate CONSTANT)

    bool shouldShowSeparator() const;
    bool shouldShowNameAndDate() const;

    bool operator==(const Message& rhs) const;

    Message& operator=(const Message& message);

    QString messageText;
    QString senderName;
    QString messageId;
    QDateTime sendTime;
    std::shared_ptr<Message> previousMessage;
};
}