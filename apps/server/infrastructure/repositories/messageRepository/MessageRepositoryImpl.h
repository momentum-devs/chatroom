#pragma once

#include <memory>
#include <odb/sqlite/database.hxx>
#include <vector>

#include "server/domain/repositories/messageRepository/MessageRepository.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapper.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapper.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapper.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapper.h"

namespace server::infrastructure
{
class MessageRepositoryImpl : public domain::MessageRepository
{
public:
    MessageRepositoryImpl(std::shared_ptr<odb::sqlite::database>, std::shared_ptr<MessageMapper>,
                          std::shared_ptr<UserMapper>, std::shared_ptr<ChannelMapper>, std::shared_ptr<GroupMapper>);

    std::shared_ptr<domain::Message> createMessage(const domain::CreateMessagePayload&) const;
    std::optional<std::shared_ptr<domain::Message>> findMessageById(const domain::FindMessageByIdPayload&) const;
    std::vector<std::shared_ptr<domain::Message>>
    findMessagesByChannelId(const domain::FindMessagesByChannelIdPayload&) const;
    std::vector<std::shared_ptr<domain::Message>>
    findMessagesByGroupId(const domain::FindMessagesByGroupIdPayload&) const;
    std::shared_ptr<domain::Message> updateMessage(const domain::UpdateMessagePayload&) const;
    void deleteMessage(const domain::DeleteMessagePayload&) const;
    unsigned countMessagesByChannelId(const domain::CountMessagesByChannelIdPayload&) const;
    unsigned countMessagesByGroupId(const domain::CountMessagesByGroupIdPayload&) const;

private:
    std::shared_ptr<odb::sqlite::database> db;
    std::shared_ptr<MessageMapper> messageMapper;
    std::shared_ptr<UserMapper> userMapper;
    std::shared_ptr<ChannelMapper> channelMapper;
    std::shared_ptr<GroupMapper> groupMapper;
};
}
