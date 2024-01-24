#include "UserMapperImpl.h"

namespace server::infrastructure
{
std::shared_ptr<domain::User> UserMapperImpl::mapToDomainUser(std::shared_ptr<User> user) const
{
    const auto id = user->getId();
    const auto email = user->getEmail();
    const auto password = user->getPassword();
    const auto nickname = user->getNickname();
    const auto active = user->isActive();
    const auto emailVerified = user->isEmailVerified();
    const auto verificationCode = user->getVerificationCode();
    const auto resetPasswordCode = user->getResetPasswordCode().null() ? std::optional<std::string>(std::nullopt) :
                                                                         user->getResetPasswordCode().get();
    const auto createdAt = user->getCreatedAt();
    const auto updatedAt = user->getUpdatedAt();
    const auto avatarUrl =
        user->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : user->getAvatarUrl().get();

    return std::make_shared<domain::User>(id, email, password, nickname, active, emailVerified, verificationCode,
                                          resetPasswordCode, createdAt, updatedAt, avatarUrl);
}

std::shared_ptr<User> UserMapperImpl::mapToPersistenceUser(std::shared_ptr<domain::User> user) const
{
    const auto id = user->getId();
    const auto email = user->getEmail();
    const auto password = user->getPassword();
    const auto nickname = user->getNickname();
    const auto active = user->isActive();
    const auto emailVerified = user->isEmailVerified();
    const auto verificationCode = user->getVerificationCode();
    const auto resetPasswordCode =
        user->getResetPasswordCode() ? *user->getResetPasswordCode() : odb::nullable<std::string>();
    const auto createdAt = user->getCreatedAt();
    const auto updatedAt = user->getUpdatedAt();
    const auto avatarUrl = user->getAvatarUrl() ? *user->getAvatarUrl() : odb::nullable<std::string>();

    return std::make_shared<User>(id, email, password, nickname, active, emailVerified, verificationCode,
                                  resetPasswordCode, createdAt, updatedAt, avatarUrl);
}
}
