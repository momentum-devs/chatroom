#pragma once

#include <gmock/gmock.h>

#include "GroupMapper.h"

namespace server::infrastructure
{
class GroupMapperMock : public GroupMapper
{
public:
    MOCK_METHOD(std::shared_ptr<domain::Group>, mapToDomainGroup, (std::shared_ptr<Group>), (const override));
    MOCK_METHOD(std::shared_ptr<Group>, mapToPersistenceGroup, (std::shared_ptr<domain::Group>),
                (const override));
};
}
