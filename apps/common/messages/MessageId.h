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
    SendFriendRequest,
    SendFriendRequestResponse,
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
    GetFriendRequests,
    GetFriendRequestsResponse,
    AcceptFriendRequests,
    RejectFriendRequests,
    ChangeFriendRequestsResponse,
    GetUserFriends,
    GetUserFriendsResponse,
    RemoveFromFriends,
    RemoveFromFriendsResponse,
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
        {MessageId::SendFriendRequest, "SendFriendRequest"},
        {MessageId::SendFriendRequestResponse, "SendFriendRequestResponse"},
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
        {MessageId::GetFriendRequests, "GetFriendRequests"},
        {MessageId::GetFriendRequestsResponse, "GetFriendRequestsResponse"},
        {MessageId::AcceptFriendRequests, "AcceptFriendRequests"},
        {MessageId::RejectFriendRequests, "RejectFriendRequests"},
        {MessageId::ChangeFriendRequestsResponse, "ChangeFriendRequestsResponse"},
        {MessageId::GetUserFriends, "GetUserFriends"},
        {MessageId::GetUserFriendsResponse, "GetUserFriendsResponse"},
        {MessageId::RemoveFromFriends, "RemoveFromFriends"},
        {MessageId::RemoveFromFriendsResponse, "RemoveFromFriendsResponse"},
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