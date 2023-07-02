#pragma once

#include "UserMapper.h"

namespace server::infrastructure
{
class UserMapperImpl : public UserMapper
{
public:
    std::shared_ptr<domain::User> mapToDomainUser(std::shared_ptr<User>) const override;
    std::shared_ptr<User> mapToPersistenceUser(std::shared_ptr<domain::User>) const override;
};
}
