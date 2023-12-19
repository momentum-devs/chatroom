#include "ReactionMapperImpl.h"

#include "gtest/gtest.h"

#include "../../userRepository/userMapper/UserMapperMock.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperMock.h"

#include "server/tests/factories/channelTestFactory/ChannelTestFactory.h"
#include "server/tests/factories/messageTestFactory/MessageTestFactory.h"
#include "server/tests/factories/reactionTestFactory/ReactionTestFactory.h"
#include "server/tests/factories/userTestFactory/UserTestFactory.h"

using namespace ::testing;
using namespace server;
using namespace server::infrastructure;
using namespace server::tests;

class ReactionMapperTest : public Test
{
public:
    UserTestFactory userTestFactory;
    MessageTestFactory messageTestFactory;
    ChannelTestFactory channelTestFactory;
    ReactionTestFactory reactionTestFactory;

    std::shared_ptr<UserMapperMock> userMapper = std::make_shared<StrictMock<UserMapperMock>>();
    std::shared_ptr<MessageMapperMock> messageMapper = std::make_shared<StrictMock<MessageMapperMock>>();

    ReactionMapperImpl reactionMapper{userMapper, messageMapper};
};

TEST_F(ReactionMapperTest, givenPersistenceReaction_shouldMapToDomainReaction)
{
    const auto user = userTestFactory.createPersistentUser();

    const auto domainUser = std::make_shared<domain::User>(
        user->getId(), user->getEmail(), user->getPassword(), user->getNickname(), user->isActive(),
        user->isEmailVerified(), user->getVerificationCode(), user->getCreatedAt(), user->getUpdatedAt(),
        user->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : user->getAvatarUrl().get());

    const auto channel = channelTestFactory.createPersistentChannel(user);

    const auto domainChannel = std::make_shared<domain::Channel>(
        channel->getId(), channel->getName(), domainUser, channel->getCreatedAt(), channel->getUpdatedAt(),
        channel->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : channel->getAvatarUrl().get());

    const auto message = messageTestFactory.createPersistentMessage(user, channel, nullptr);

    const auto domainMessage =
        std::make_shared<domain::Message>(message->getId(), message->getContent(), domainUser, domainChannel, nullptr,
                                          message->getCreatedAt(), message->getUpdatedAt());

    const auto reaction = reactionTestFactory.createPersistentReaction(user, message);

    EXPECT_CALL(*userMapper, mapToDomainUser(user)).WillOnce(Return(domainUser));

    EXPECT_CALL(*messageMapper, mapToDomainMessage(message)).WillOnce(Return(domainMessage));

    const auto domainReaction = reactionMapper.mapToDomainReaction(*reaction);

    ASSERT_EQ(domainReaction.getId(), reaction->getId());
    ASSERT_EQ(domainReaction.getUser(), domainUser);
    ASSERT_EQ(domainReaction.getMessage(), domainMessage);
}
