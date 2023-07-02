#pragma once

#include <gmock/gmock.h>

#include "UserMapper.h"

namespace server::infrastructure
{
class UserMapperMock : public UserMapper
{
public:
    MOCK_METHOD(std::shared_ptr<domain::User>, mapToDomainUser, (std::shared_ptr<User>), (const override));
    MOCK_METHOD(std::shared_ptr<User>, mapToPersistenceUser, (std::shared_ptr<domain::User>), (const override));
};
}
