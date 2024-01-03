#include "UserSettingsController.h"

#include <nlohmann/json.hpp>

#include "fmt/format.h"
#include "loguru.hpp"

namespace client::gui
{
UserSettingsController::UserSettingsController(std::shared_ptr<api::Session> sessionInit,
                                               const StateFactory& stateFactoryInit,
                                               std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void UserSettingsController::activate()
{
    session->addMessageHandler({common::messages::MessageId::UpdateUserResponse, updateUserResponseHandlerName,
                                [this](const auto& msg) { handleUpdateUserResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::DeleteUserResponse, deleteUserResponseHandlerName,
                                [this](const auto& msg) { handleDeleteUserResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::GetUserDataResponse, getUserDataResponseHandlerName,
                                [this](const auto& msg) { handleGetUserDataResponse(msg); }});

    auto userName = dynamic_cast<types::User*>(session->getUser())->property("nickname").toString();
    auto userEmail = dynamic_cast<types::User*>(session->getUser())->property("email").toString();

    emit setUserData(userEmail, userName);
}

void UserSettingsController::deactivate()
{
    session->removeMessageHandler({common::messages::MessageId::UpdateUserResponse, updateUserResponseHandlerName});

    session->removeMessageHandler({common::messages::MessageId::DeleteUserResponse, deleteUserResponseHandlerName});

    session->removeMessageHandler({common::messages::MessageId::GetUserDataResponse, getUserDataResponseHandlerName});

    nextState = std::nullopt;
}

void UserSettingsController::changeNickname(const QString& nickname)
{
    LOG_S(INFO) << "Change user nickname to " << nickname.toStdString();

    nlohmann::json data{
        {"nickname", nickname.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::UpdateUser, data);
}

void UserSettingsController::changePassword(const QString& password)
{
    LOG_S(INFO) << "Change user password";

    nlohmann::json data{
        {"password", password.toStdString()},
    };

    session->sendMessage(common::messages::MessageId::UpdateUser, data);
}

void UserSettingsController::deleteUser()
{
    LOG_S(INFO) << "Delete user";

    session->sendMessage(common::messages::MessageId::DeleteUser, {});

    nextState = stateFactory.createDefaultState();
}

void UserSettingsController::goBack()
{
    stateMachine->returnToThePreviousState();
}

void UserSettingsController::handleUpdateUserResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle update user response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        auto errorMessage = fmt::format("Error while updating user: {}", responseJson.at("error").get<std::string>());

        emit updateUserFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }

    if (responseJson.is_array() and responseJson.at(0).get<std::string>() == "ok")
    {
        LOG_S(INFO) << "Successfully updated user";

        session->sendMessage(common::messages::MessageId::GetUserData, {});
    }
}

void UserSettingsController::handleDeleteUserResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle delete user response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        auto errorMessage = fmt::format("Error while updating user: {}", responseJson.at("error").get<std::string>());

        emit deleteUserFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }

    if (responseJson.is_array() and responseJson.at(0).get<std::string>() == "ok")
    {
        LOG_S(INFO) << "Successfully deleted user";

        session->storeToken("");

        stateMachine->clear(nextState.value());
    }
}

void UserSettingsController::handleGetUserDataResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle get user data response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << fmt::format("Error while getting user data: {}", responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data") and responseJson.at("data").contains("email") and
        responseJson.at("data").contains("nickname"))
    {
        LOG_S(INFO) << "Successfully obtain user data";

        auto user = dynamic_cast<types::User*>(session->getUser());

        auto userNickname = QString::fromStdString(responseJson.at("data").at("nickname").get<std::string>());
        auto userEmail = QString::fromStdString(responseJson.at("data").at("email").get<std::string>());
        auto userIsVerified = user->property("isVerified").toBool();
        auto userIsOnline = user->property("isOnline").toBool();
        auto userId = user->property("userId").toString();

        emit setUserData(userEmail, userNickname);

        session->storeUser({userIsVerified, userIsOnline, userNickname, userId, userEmail});
    }
    else
    {
        LOG_S(ERROR) << "Unknown error while getting user data";
    }
}

void UserSettingsController::logout()
{
    LOG_S(INFO) << "Handle logout";

    session->logout();

    stateMachine->clear(stateFactory.createLoginState());
}
}
