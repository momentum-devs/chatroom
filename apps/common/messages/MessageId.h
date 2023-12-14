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
    SendFriendInvitation,
    SendFriendInvitationResponse,
    GetUserData,
    GetUserDataResponse,
    UpdateUser,
    UpdateUserResponse,
    DeleteUser,
    DeleteUserResponse,
    VerifyUser,
    VerifyUserResponse,
    Logout,
    SendChannelInvitation,
    SendChannelInvitationResponse,
    LeftTheChannel,
    LeftTheChannelResponse,
    DeleteTheChannel,
    DeleteTheChannelResponse,
    GetUserChannelInvitations,
    GetUserChannelInvitationsResponse,
    AcceptChannelInvitation,
    RejectChannelInvitation,
    ChangeChannelInvitationResponse,
    GetFriendInvitations,
    GetFriendInvitationsResponse,
    AcceptFriendInvitations,
    RejectFriendInvitations,
    ChangeFriendInvitationsResponse,
    GetUserFriends,
    GetUserFriendsResponse,
    RemoveFromFriends,
    RemoveFromFriendsResponse,
    GetChannelMembers,
    GetChannelMembersResponse,
    GetChannelMessages,
    GetChannelMessagesResponse,
    SendChannelMessage,
    SendChannelMessageResponse,
    GetPrivateMessages,
    GetPrivateMessagesResponse,
    SendPrivateMessage,
    SendPrivateMessageResponse,
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
        {MessageId::SendFriendInvitation, "SendFriendInvitation"},
        {MessageId::SendFriendInvitationResponse, "SendFriendInvitationResponse"},
        {MessageId::GetUserData, "GetUserData"},
        {MessageId::GetUserDataResponse, "GetUserDataResponse"},
        {MessageId::UpdateUser, "UpdateUser"},
        {MessageId::UpdateUserResponse, "UpdateUserResponse"},
        {MessageId::DeleteUser, "DeleteUser"},
        {MessageId::DeleteUserResponse, "DeleteUserResponse"},
        {MessageId::VerifyUser, "VerifyUser"},
        {MessageId::VerifyUserResponse, "VerifyUserResponse"},
        {MessageId::Logout, "Logout"},
        {MessageId::SendChannelInvitation, "SendChannelInvitation"},
        {MessageId::SendChannelInvitationResponse, "SendChannelInvitationResponse"},
        {MessageId::LeftTheChannel, "LeftTheChannel"},
        {MessageId::LeftTheChannelResponse, "LeftTheChannelResponse"},
        {MessageId::DeleteTheChannel, "DeleteTheChannel"},
        {MessageId::DeleteTheChannelResponse, "DeleteTheChannelResponse"},
        {MessageId::GetUserChannelInvitations, "GetUserChannelInvitations"},
        {MessageId::GetUserChannelInvitationsResponse, "GetUserChannelInvitationsResponse"},
        {MessageId::AcceptChannelInvitation, "AcceptChannelInvitation"},
        {MessageId::RejectChannelInvitation, "RejectChannelInvitation"},
        {MessageId::ChangeChannelInvitationResponse, "ChangeChannelInvitationResponse"},
        {MessageId::GetFriendInvitations, "GetFriendInvitations"},
        {MessageId::GetFriendInvitationsResponse, "GetFriendInvitationsResponse"},
        {MessageId::AcceptFriendInvitations, "AcceptFriendInvitations"},
        {MessageId::RejectFriendInvitations, "RejectFriendInvitations"},
        {MessageId::ChangeFriendInvitationsResponse, "ChangeFriendInvitationsResponse"},
        {MessageId::GetUserFriends, "GetUserFriends"},
        {MessageId::GetUserFriendsResponse, "GetUserFriendsResponse"},
        {MessageId::RemoveFromFriends, "RemoveFromFriends"},
        {MessageId::RemoveFromFriendsResponse, "RemoveFromFriendsResponse"},
        {MessageId::GetChannelMembers, "GetChannelMembers"},
        {MessageId::GetChannelMembersResponse, "GetChannelMembersResponse"},
        {MessageId::GetChannelMessages, "GetChannelMessages"},
        {MessageId::GetChannelMessagesResponse, "GetChannelMessagesResponse"},
        {MessageId::SendChannelMessage, "SendChannelMessage"},
        {MessageId::SendChannelMessageResponse, "SendChannelMessageResponse"},
        {MessageId::GetPrivateMessages, "GetPrivateMessages"},
        {MessageId::GetPrivateMessagesResponse, "GetPrivateMessagesResponse"},
        {MessageId::SendPrivateMessage, "SendPrivateMessage"},
        {MessageId::SendPrivateMessageResponse, "SendPrivateMessageResponse"},
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