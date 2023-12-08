#pragma once

#include <QObject>

namespace client::types
{
class Message : public QObject
{
    Q_OBJECT
public:
    bool fromUser;
    QString messageText;
    QString userId;
    QString messageId;
    QString sendTime;

    Q_PROPERTY(bool fromUser MEMBER fromUser)
    Q_PROPERTY(QString messageText MEMBER messageText)
    Q_PROPERTY(QString userId MEMBER userId)
    Q_PROPERTY(QString messageId MEMBER messageId)
    Q_PROPERTY(QString sendTime MEMBER sendTime)
};
}