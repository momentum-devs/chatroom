#pragma once

#include <gmock/gmock.h>

#include "GroupRepository.h"

namespace server::domain
{
class GroupRepositoryMock : public GroupRepository
{
public:
    MOCK_METHOD(std::shared_ptr<Group>, createGroup, (const CreateGroupPayload&), (const override));
    MOCK_METHOD(std::optional<std::shared_ptr<Group>>, findGroupById, (const FindGroupByIdPayload&), (const override));
    MOCK_METHOD(void, deleteGroup, (const DeleteGroupPayload&), (const override));
};
}
