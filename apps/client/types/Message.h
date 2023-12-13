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
            const QDateTime& sendTimeInit);
    Message(const Message& message);

    Q_PROPERTY(QString messageText MEMBER messageText CONSTANT)
    Q_PROPERTY(QString senderName MEMBER senderName CONSTANT)
    Q_PROPERTY(QString messageId MEMBER messageId CONSTANT)
    Q_PROPERTY(QDateTime sendTime MEMBER sendTime CONSTANT)
    Q_PROPERTY(bool showSeparator MEMBER showSeparator CONSTANT)
    Q_PROPERTY(bool showNameAndDate MEMBER showNameAndDate CONSTANT)

    bool operator==(const Message& rhs) const;

    Message& operator=(const Message& message);

    QString messageText;
    QString senderName;
    QString messageId;
    QDateTime sendTime;
    bool showSeparator = false;
    bool showNameAndDate = false;
};
}