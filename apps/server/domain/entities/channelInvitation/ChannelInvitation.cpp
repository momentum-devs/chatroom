#include "ChannelInvitation.h"

#include <utility>

namespace server::domain
{
ChannelInvitation::ChannelInvitation(std::string idInit, std::shared_ptr<User> senderInit,
                                     std::shared_ptr<User> recipientInit, std::shared_ptr<Channel> channelInit,
                                     std::string createdAtInit)
    : id{std::move(idInit)},
      createdAt{std::move(createdAtInit)},
      sender{std::move(senderInit)},
      recipient{std::move(recipientInit)},
      channel{std::move(channelInit)}
{
}

std::string ChannelInvitation::getId() const
{
    return id;
}

std::shared_ptr<User> ChannelInvitation::getSender() const
{
    return sender;
}

std::shared_ptr<User> ChannelInvitation::getRecipient() const
{
    return recipient;
}

std::shared_ptr<Channel> ChannelInvitation::getChannel() const
{
    return channel;
}

std::string ChannelInvitation::getCreatedAt() const
{
    return createdAt;
}

}
