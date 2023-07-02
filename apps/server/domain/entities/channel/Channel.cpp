#include "Channel.h"

#include <utility>

namespace server::domain
{
Channel::Channel(std::string idInit, std::string nameInit, std::string creatorIdInit, std::string createdAtInit,
                 std::string updatedAtInit)
    : id{std::move(idInit)},
      name{std::move(nameInit)},
      creatorId{std::move(creatorIdInit)},
      createdAt{std::move(createdAtInit)},
      updatedAt{std::move(updatedAtInit)}
{
}

std::string Channel::getId() const
{
    return id;
}

std::string Channel::getName() const
{
    return name;
}

std::string Channel::getCreatorId() const
{
    return creatorId;
}

std::string Channel::getCreatedAt() const
{
    return createdAt;
}

std::string Channel::getUpdatedAt() const
{
    return updatedAt;
}

bool Channel::operator==(const Channel& channel) const
{
    auto tieStruct = [](const Channel& channel)
    { return std::tie(channel.id, channel.creatorId, channel.name, channel.createdAt, channel.updatedAt); };

    return tieStruct(*this) == tieStruct(channel);
}
}
