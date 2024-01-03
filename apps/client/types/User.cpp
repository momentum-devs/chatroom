#include "User.h"

namespace client::types
{
User::User(bool isActivatedInit, bool isOnlineInit, const QString& nicknameInit, const QString& userIdInit,
           const QString& emailInit)
    : isActivated{isActivatedInit}, isOnline{isOnlineInit}, nickname{nicknameInit}, userId{userIdInit}, email{emailInit}
{
}

User::User(const User& user)
    : QObject(),
      isActivated{user.isActivated},
      isOnline{user.isOnline},
      nickname{user.nickname},
      userId{user.userId},
      email{user.email}
{
}

bool User::operator==(const User& rhs) const
{
    auto tieStruct = [](const User& user)
    { return std::tie(user.isActivated, user.isOnline, user.nickname, user.userId, user.email); };

    return tieStruct(*this) == tieStruct(rhs);
}

User& User::operator=(const User& user)
{
    isActivated = user.isActivated;
    isOnline = user.isOnline;
    nickname = user.nickname;
    userId = user.userId;
    email = user.email;

    return *this;
}

void User::update(const User& user)
{
    isActivated = user.isActivated;
    isOnline = user.isOnline;
    nickname = user.nickname;
    userId = user.userId;
    email = user.email;

    emit userUpdated();
}
}
