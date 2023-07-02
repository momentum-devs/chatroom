#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

namespace common::messages
{
enum class MessageId : unsigned char
{
    Error,
    Register,
    RegisterResponse,
    Login,
    LoginResponse,
    CreateChannel,
    CreateChannelResponse,
    GetUserChannels,
    GetUserChannelsResponse,
};

inline std::string toString(MessageId messageId)
{
    static const std::unordered_map<MessageId, std::string> messageIdsToStringMapping{
        {MessageId::Error, "Error"},
        {MessageId::Register, "Register"},
        {MessageId::RegisterResponse, "RegisterResponse"},
        {MessageId::Login, "Login"},
        {MessageId::LoginResponse, "LoginResponse"},
        {MessageId::CreateChannel, "CreateChannel"},
        {MessageId::CreateChannelResponse, "CreateChannelResponse"},
        {MessageId::GetUserChannels, "GetUserChannels"},
        {MessageId::GetUserChannelsResponse, "GetUserChannelsResponse"},
    };

    try
    {
        return messageIdsToStringMapping.at(messageId);
    }
    catch (const std::out_of_range& e)
    {
        return "Undefined";
    }
}

inline std::ostream& operator<<(std::ostream& os, MessageId messageId)
{
    return os << "MessageId::" << toString(messageId) << ", value: " << static_cast<unsigned int>(messageId);
}
}