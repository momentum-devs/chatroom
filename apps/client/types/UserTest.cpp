#include "User.h"

#include <gtest/gtest.h>
#include <QSignalSpy>

#include "faker-cxx/Internet.h"
#include "faker-cxx/String.h"

using namespace ::testing;
using namespace client::types;

class UserTest : public Test
{
};

TEST_F(UserTest, shouldCreateUser)
{
    const auto isActivated = true;
    const auto isOnline = true;
    const QString nickname = faker::Internet::username().c_str();
    const QString userId = faker::String::uuid().c_str();
    const QString email = faker::Internet::email().c_str();

    const User user{isActivated, isOnline, nickname, userId, email};

    ASSERT_EQ(isActivated, user.property("isActivated").toBool());
    ASSERT_EQ(isOnline, user.property("isOnline").toBool());
    ASSERT_EQ(nickname, user.property("nickname").toString());
    ASSERT_EQ(userId, user.property("userId").toString());
    ASSERT_EQ(email, user.property("email").toString());
}

TEST_F(UserTest, shouldUpdateUserWithSignalUserUpdated)
{
    const auto isActivated = true;
    const auto isOnline = true;
    const QString nickname = faker::Internet::username().c_str();
    const QString userId = faker::String::uuid().c_str();
    const QString email = faker::Internet::email().c_str();

    User user{isActivated, isOnline, nickname, userId, email};

    const auto newIsActivated = false;
    const auto newIsOnline = false;
    const QString newNickname = faker::Internet::username().c_str();
    const QString newUserId = faker::String::uuid().c_str();
    const QString newEmail = faker::Internet::email().c_str();

    QSignalSpy spy(&user, &User::userUpdated);

    user.update({newIsActivated, newIsOnline, newNickname, newUserId, newEmail});

    ASSERT_EQ(1, spy.count());
    ASSERT_EQ(newIsActivated, user.property("isActivated").toBool());
    ASSERT_EQ(newIsOnline, user.property("isOnline").toBool());
    ASSERT_EQ(newNickname, user.property("nickname").toString());
    ASSERT_EQ(newUserId, user.property("userId").toString());
    ASSERT_EQ(newEmail, user.property("email").toString());
}

TEST_F(UserTest, twoDifferentUsersShouldNotBeEqual)
{
    const auto isActivated = true;
    const auto isOnline = true;
    const QString nickname = faker::Internet::username().c_str();
    const QString userId = faker::String::uuid().c_str();
    const QString email = faker::Internet::email().c_str();

    const User user{isActivated, isOnline, nickname, userId, email};

    const auto newIsActivated = false;
    const auto newIsOnline = false;
    const QString newNickname = faker::Internet::username().c_str();
    const QString newUserId = faker::String::uuid().c_str();
    const QString newEmail = faker::Internet::email().c_str();

    const User newUser{newIsActivated, newIsOnline, newNickname, newUserId, newEmail};

    ASSERT_NE(user, newUser);
}

TEST_F(UserTest, copiedUserShouldBeEqual)
{
    const auto isActivated = true;
    const auto isOnline = true;
    const QString nickname = faker::Internet::username().c_str();
    const QString userId = faker::String::uuid().c_str();
    const QString email = faker::Internet::email().c_str();

    const User user{isActivated, isOnline, nickname, userId, email};

    const User newUser{user};

    ASSERT_EQ(user, newUser);
}

TEST_F(UserTest, assignedUserShouldBeEqual)
{
    const auto isActivated = true;
    const auto isOnline = true;
    const QString nickname = faker::Internet::username().c_str();
    const QString userId = faker::String::uuid().c_str();
    const QString email = faker::Internet::email().c_str();

    const User user{isActivated, isOnline, nickname, userId, email};

    User newUser{};

    newUser = user;

    ASSERT_EQ(user, newUser);
}