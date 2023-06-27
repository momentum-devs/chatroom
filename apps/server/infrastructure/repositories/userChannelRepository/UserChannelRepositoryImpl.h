#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapper.h"

namespace server::infrastructure
{
class UserChannelRepositoryImpl : public domain::UserChannelRepository
{
public:
    UserChannelRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::unique_ptr<UserChannelMapper>);

    domain::UserChannel createUserChannel(const domain::CreateUserChannelPayload&) const;
    std::optional<domain::UserChannel> findUserChannelById(const domain::FindUserChannelByIdPayload&) const;
    std::vector<domain::UserChannel> findUsersChannelsByUserId(const domain::FindUsersChannelsByUserIdPayload&) const;
    std::vector<domain::UserChannel>
    findUsersChannelsByChannelId(const domain::FindUsersChannelsByChannelIdPayload&) const;
    void deleteUserChannel(const domain::DeleteUserChannelPayload&) const;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::unique_ptr<UserChannelMapper> userChannelMapper;
};
}
