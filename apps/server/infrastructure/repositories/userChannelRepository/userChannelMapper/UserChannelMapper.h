#pragma once

#include "../../../../domain/entities/userChannel/UserChannel.h"
#include "../../../database/tables/userChannelTable/UserChannel.h"

namespace server::infrastructure
{
class UserChannelMapper
{
public:
    virtual ~UserChannelMapper() = default;

    virtual domain::UserChannel mapToDomainUserChannel(const UserChannel&) const = 0;
};
}
