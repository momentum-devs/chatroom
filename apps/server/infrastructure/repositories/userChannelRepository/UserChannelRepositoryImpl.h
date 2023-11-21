#pragma once

#include <memory>
#include <odb/pgsql/database.hxx>
#include <vector>

#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class UserChannelRepositoryImpl : public domain::UserChannelRepository
{
public:
    UserChannelRepositoryImpl(std::shared_ptr<odb::pgsql::database>, std::shared_ptr<UserChannelMapper>,
                              std::shared_ptr<UserMapper>, std::shared_ptr<ChannelMapper>);

    domain::UserChannel createUserChannel(const domain::CreateUserChannelPayload&) const;
    std::optional<domain::UserChannel> findUserChannel(const domain::FindUserChannelPayload&) const;
    std::optional<domain::UserChannel> findUserChannelById(const domain::FindUserChannelByIdPayload&) const;
    std::vector<domain::UserChannel> findUsersChannelsByUserId(const domain::FindUsersChannelsByUserIdPayload&) const;
    std::vector<domain::UserChannel>
    findUsersChannelsByChannelId(const domain::FindUsersChannelsByChannelIdPayload&) const;
    domain::UserChannel updateUserChannel(const domain::UpdateUserChannelPayload&) const;
    void deleteUserChannel(const domain::DeleteUserChannelPayload&) const;

private:
    std::shared_ptr<odb::pgsql::database> db;
    std::shared_ptr<UserChannelMapper> userChannelMapper;
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<ChannelMapper> channelMapper;
};
}
