#include "fmt/core.h"
#include "gtest/gtest.h"

#include "MessageSerializerImpl.h"
#include "errors/InvalidChecksumError.h"

using namespace common;
using namespace common::messages;

namespace
{
const auto serializedMessageZeroes = bytes::Bytes{
        0, // id
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // token
        0,0,0,0, // payload
        0,0,0,0 // checksum
    };
const auto idZeroes = static_cast<MessageId>(0);
const auto tokenZeroes = bytes::Bytes{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const auto payloadZeroes = bytes::Bytes{0,0,0,0};
const auto checksumZeroes = bytes::Bytes{0,0,0,0};
const auto messageZeroes = Message{idZeroes, tokenZeroes, payloadZeroes};

const auto invalidSerializedMesage = bytes::Bytes{
    0, // id
    1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // token
    0,0,0,0, // payload
    0,0,0,0 // checksum
};

const auto validSerializedMesage = bytes::Bytes{
    0, // id
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // token
    1,2,3,4, // payload
    4,1,2,3 // checksum
};
const auto validPayload = bytes::Bytes{1,2,3,4};
const auto validChecksum = bytes::Bytes{4,1,2,3};
const auto validMessage = Message{idZeroes, tokenZeroes, validPayload};
}

using namespace ::testing;

class MessageSerializerImplTest : public Test
{
public:
    MessageSerializerImpl messageSerializer{};
};

TEST_F(MessageSerializerImplTest, giveSerializedMesageOfZeroes_shouldDeserializeMessage)
{
    const auto deserializedMessage = messageSerializer.deserialize(serializedMessageZeroes);

    EXPECT_EQ(deserializedMessage.id, idZeroes);
    EXPECT_EQ(deserializedMessage.token, tokenZeroes);
    EXPECT_EQ(deserializedMessage.payload, payloadZeroes);
    EXPECT_EQ(deserializedMessage.calculateCheckSum(), checksumZeroes);
    EXPECT_EQ(deserializedMessage, messageZeroes);
}

TEST_F(MessageSerializerImplTest, giveInvalidSerializedMesage_shouldThrowInvalidChecksumError)
{
    EXPECT_THROW(messageSerializer.deserialize(invalidSerializedMesage), InvalidChecksumError);
}

TEST_F(MessageSerializerImplTest, giveValidSerializedMesage_shouldDeserializeMessage)
{
    const auto deserializedMessage = messageSerializer.deserialize(validSerializedMesage);

    EXPECT_EQ(deserializedMessage.id, idZeroes);
    EXPECT_EQ(deserializedMessage.token, tokenZeroes);
    EXPECT_EQ(deserializedMessage.payload, validPayload);
    EXPECT_EQ(deserializedMessage.calculateCheckSum(), validChecksum);
    EXPECT_EQ(deserializedMessage, validMessage);
}

TEST_F(MessageSerializerImplTest, giveValidMessage_shouldSerializeMessage)
{
    const auto serializedMessage= messageSerializer.serialize(validMessage);

    EXPECT_EQ(serializedMessage, validSerializedMesage);
}

TEST_F(MessageSerializerImplTest, giveValidMessage_redeserializedMessageShouldBeSame)
{
    const auto serializedMessage= messageSerializer.serialize(validMessage);
    const auto deserializedMessage = messageSerializer.deserialize(serializedMessage);

    EXPECT_EQ(deserializedMessage, validMessage);
}

TEST_F(MessageSerializerImplTest, giveValidSerializedMessage_reserializedMessageShouldBeSame)
{
    const auto deserializedMessage = messageSerializer.deserialize(validSerializedMesage);
    const auto serializedMessage= messageSerializer.serialize(deserializedMessage);

    EXPECT_EQ(deserializedMessage, validMessage);
}