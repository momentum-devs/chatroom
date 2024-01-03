#include "LoginController.h"

#include <nlohmann/json.hpp>

#include "loguru.hpp"

namespace client::gui
{
LoginController::LoginController(std::shared_ptr<api::Session> sessionInit, const StateFactory& stateFactoryInit,
                                 std::shared_ptr<StateMachine> stateMachineInit)
    : session{std::move(sessionInit)}, stateFactory{stateFactoryInit}, stateMachine{std::move(stateMachineInit)}
{
}

void LoginController::loginRequest(const QString& email, const QString& password)
{
    nlohmann::json payload{
        {"email", email.toStdString()},
        {"password", password.toStdString()},
    };

    common::messages::Message message{common::messages::MessageId::Login, common::bytes::Bytes{payload.dump()}};

    session->sendMessage(message);

    LOG_S(INFO) << fmt::format("Sent login request for user {}", static_cast<std::string>(message.payload));

    privateMessageState = stateFactory.createPrivateMessagesState();
    verificationState = stateFactory.createVerifyUserState();
}

void LoginController::goToRegisterState()
{
    stateMachine->addNextState(stateFactory.createRegisterState());
}

void LoginController::activate()
{
    session->addMessageHandler({common::messages::MessageId::LoginResponse, loginResponseHandlerName,
                                [this](const auto& msg) { handleLoginResponse(msg); }});

    session->addMessageHandler({common::messages::MessageId::GetUserDataResponse, getUserDataHandlerName,
                                [this](const auto& msg) { handleGetUSerDataResponse(msg); }});
}

void LoginController::deactivate()
{
    session->removeMessageHandler({common::messages::MessageId::LoginResponse, loginResponseHandlerName});

    session->removeMessageHandler({common::messages::MessageId::GetUserDataResponse, getUserDataHandlerName});
}

void LoginController::handleLoginResponse(const common::messages::Message& message)
{
    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    LOG_S(INFO) << "Handle login response";

    if (responseJson.contains("error"))
    {
        auto errorMessage = fmt::format("Error while logging: {}", responseJson.at("error").get<std::string>());

        emit loginFailure(QString::fromStdString(errorMessage));

        LOG_S(ERROR) << errorMessage;
    }

    if (responseJson.contains("token"))
    {
        LOG_S(INFO) << "Successfully logged";

        session->storeToken(responseJson.at("token").get<std::string>());

        session->sendMessage(common::messages::MessageId::GetUserData, {});
    }
}

void LoginController::handleGetUSerDataResponse(const common::messages::Message& message)
{
    LOG_S(INFO) << "Handle get user data response";

    auto responsePayload = static_cast<std::string>(message.payload);

    auto responseJson = nlohmann::json::parse(responsePayload);

    if (responseJson.contains("error"))
    {
        LOG_S(ERROR) << fmt::format("Error while getting user channels: {}",
                                    responseJson.at("error").get<std::string>());
    }

    if (responseJson.contains("data") and responseJson.at("data").contains("verified") and
        responseJson.at("data").contains("nickname") and responseJson.at("data").contains("email"))
    {

        session->storeUser({responseJson.at("data").at("verified").get<bool>(), true,
                            responseJson.at("data").at("nickname").get<std::string>().c_str(), "",
                            responseJson.at("data").at("email").get<std::string>().c_str()});

        if (responseJson.at("data").at("verified").get<bool>())
        {
            LOG_S(INFO) << "User verified, go to private message state";

            stateMachine->addNextState(privateMessageState);
        }
        else
        {
            LOG_S(INFO) << "User not verified, go to verification state";

            stateMachine->addNextState(verificationState);
        }
    }
    else
    {
        LOG_S(ERROR) << "Wrong user data format";
    }
}
}
