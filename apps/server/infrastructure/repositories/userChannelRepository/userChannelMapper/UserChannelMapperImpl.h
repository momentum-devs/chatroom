#pragma once

#include "UserChannelMapper.h"

namespace server::infrastructure
{
class UserChannelMapperImpl : public UserChannelMapper
{
public:
    domain::UserChannel mapToDomainUserChannel(const UserChannel&) const override;
};
}
