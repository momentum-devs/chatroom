#pragma once

#include <QObject>

namespace client::types
{
class User : public QObject
{
    Q_OBJECT
public:
    User(bool isActivatedInit = false, bool isOnlineInit = false, const QString& nicknameInit = "",
         const QString& userIdInit = "", const QString& emailInit = "");
    User(const User& user);

    Q_PROPERTY(bool isActivated MEMBER isActivated NOTIFY userUpdated)
    Q_PROPERTY(bool isOnline MEMBER isOnline NOTIFY userUpdated)
    Q_PROPERTY(QString nickname MEMBER nickname NOTIFY userUpdated)
    Q_PROPERTY(QString userId MEMBER userId NOTIFY userUpdated)
    Q_PROPERTY(QString email MEMBER email NOTIFY userUpdated)

    bool operator==(const User& rhs) const;
    User& operator=(const User& user);
    void update(const User& user);

signals:
    void userUpdated();

private:
    bool isActivated;
    bool isOnline;
    QString nickname;
    QString userId;
    QString email;
};
}