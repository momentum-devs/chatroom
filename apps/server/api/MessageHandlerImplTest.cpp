// #include "MessageHandlerImpl.h"
//
// #include <format>
// #include <gtest/gtest.h>
// #include <regex>
//
// #include "server/application/commandHandlers/createChannelCommandHandler/CreateChannelCommandHandlerMock.h"
// #include "server/application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandlerMock.h"
// #include "server/application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandlerMock.h"
// #include
// "server/application/queryHandlers/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerMock.h"
// #include "server/application/services/tokenService/TokenServiceMock.h"
//
// #include "nlohmann/json.hpp"
//
// using namespace ::testing;
// using namespace server::api;
//
// namespace
//{
// const std::string id = "id";
// const std::string validEmail = "abc@abc.com";
// const std::string validPassword = "password";
// const bool active = true;
// const bool emailVerified = false;
// const std::string createdAt = "2023-06-16";
// const std::string updatedAt = "2023-06-16";
// const server::application::RegisterUserCommandHandlerPayload validRegisterUserCommandHandlerPayload{validEmail,
//                                                                                                     validPassword};
// const server::application::RegisterUserCommandHandlerResult validRegisterUserCommandHandlerResult{
//     {id, validEmail, validPassword, validPassword, createdAt, updatedAt}};
// common::bytes::Bytes validRegisterPayload{
//     std::format(R"({{"email":"{}","password":"{}"}})", validEmail, validPassword)};
// common::messages::Message validRegisterMessage{common::messages::MessageId::Register, validRegisterPayload};
// common::bytes::Bytes okResponse{R"(["ok"])"};
//
// const std::string invalidEmail = "invalidEmail";
// const std::string invalidPassword = "invalidPassword";
// common::bytes::Bytes payloadWithInvalidEmail{
//     std::format(R"({{"email":"{}","password":"{}"}})", invalidEmail, invalidPassword)};
// common::messages::Message registerMessagedWithInvalidEmail{common::messages::MessageId::Register,
//                                                            payloadWithInvalidEmail};
// common::bytes::Bytes wrongEmailResponse{nlohmann::json{{"error", "wrong email address"}}.dump()};
//
// const std::string errorMessage{"errorMessage"};
// std::runtime_error exception{errorMessage};
// common::bytes::Bytes exceptionResponse{nlohmann::json{{"error", errorMessage}}.dump()};
//
// const std::string token = "token123";
// const server::application::LoginUserCommandHandlerPayload validLoginUserCommandHandlerPayload{validEmail,
//                                                                                               validPassword};
// const server::application::LoginUserCommandHandlerResult validLoginUserCommandHandlerResult{token};
// common::bytes::Bytes validLoginPayload{std::format(R"({{"email":"{}","password":"{}"}})", validEmail,
// validPassword)}; common::messages::Message validLoginMessage{common::messages::MessageId::Login, validLoginPayload};
// common::bytes::Bytes tokenResponse{std::format(R"({{"token":"{}"}})", token)};
//
// common::messages::Message loginMessagedWithInvalidEmail{common::messages::MessageId::Login, payloadWithInvalidEmail};
// }
//
// class MessageHandlerImplTest : public Test
//{
// public:
//     std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
//         std::make_shared<StrictMock<server::application::TokenServiceMock>>();
//
//     std::unique_ptr<server::application::RegisterUserCommandHandlerMock> registerUserCommandHandlerMockInit =
//         std::make_unique<StrictMock<server::application::RegisterUserCommandHandlerMock>>();
//     server::application::RegisterUserCommandHandlerMock* registerUserCommandHandlerMock =
//         registerUserCommandHandlerMockInit.get();
//
//     std::unique_ptr<server::application::LoginUserCommandHandlerMock> loginUserCommandHandlerMockInit =
//         std::make_unique<StrictMock<server::application::LoginUserCommandHandlerMock>>();
//     server::application::LoginUserCommandHandlerMock* loginUserCommandHandlerMock =
//         loginUserCommandHandlerMockInit.get();
//
//     std::unique_ptr<server::application::CreateChannelCommandHandlerMock> createChannelCommandHandlerMockInit =
//         std::make_unique<StrictMock<server::application::CreateChannelCommandHandlerMock>>();
//     server::application::CreateChannelCommandHandlerMock* createChannelCommandHandlerMock =
//         createChannelCommandHandlerMockInit.get();
//
//     std::unique_ptr<server::application::FindChannelsToWhichUserBelongsQueryHandlerMock>
//         findChannelsToWhichUserBelongsQueryHandlerMockInit =
//             std::make_unique<StrictMock<server::application::FindChannelsToWhichUserBelongsQueryHandlerMock>>();
//     server::application::FindChannelsToWhichUserBelongsQueryHandlerMock*
//         findChannelsToWhichUserBelongsQueryHandlerMock = findChannelsToWhichUserBelongsQueryHandlerMockInit.get();
//
//     MessageHandlerImpl messageHandler{
//         tokenServiceMock, std::move(registerUserCommandHandlerMockInit), std::move(loginUserCommandHandlerMockInit),
//         std::move(createChannelCommandHandlerMockInit),
//         std::move(findChannelsToWhichUserBelongsQueryHandlerMockInit)};
// };
//
// TEST_F(MessageHandlerImplTest, handleRegisterValidMessage)
//{
//     EXPECT_CALL(*registerUserCommandHandlerMock, execute(validRegisterUserCommandHandlerPayload))
//         .WillOnce(Return(validRegisterUserCommandHandlerResult));
//
//     auto response = messageHandler.handleMessage(validRegisterMessage);
//
//     EXPECT_EQ(response.id, common::messages::MessageId::RegisterResponse);
//     EXPECT_EQ(response.payload, okResponse);
// }
//
// TEST_F(MessageHandlerImplTest, handleRegisterValidMessageRegisterCommandThrowException)
//{
//     EXPECT_CALL(*registerUserCommandHandlerMock, execute(validRegisterUserCommandHandlerPayload))
//         .WillOnce(Throw(exception));
//
//     auto response = messageHandler.handleMessage(validRegisterMessage);
//
//     EXPECT_EQ(response.id, common::messages::MessageId::RegisterResponse);
//     EXPECT_EQ(response.payload, exceptionResponse);
// }
//
// TEST_F(MessageHandlerImplTest, handleRegisterMessageWithInvalidEmail)
//{
//     auto response = messageHandler.handleMessage(registerMessagedWithInvalidEmail);
//
//     EXPECT_EQ(response.id, common::messages::MessageId::RegisterResponse);
//     EXPECT_EQ(response.payload, wrongEmailResponse);
// }
//
// TEST_F(MessageHandlerImplTest, handleLoginValidMessage)
//{
//     EXPECT_CALL(*loginUserCommandHandlerMock, execute(validLoginUserCommandHandlerPayload))
//         .WillOnce(Return(validLoginUserCommandHandlerResult));
//     EXPECT_CALL(*tokenServiceMock, getUserIdFromToken(token)).WillOnce(Return(id));
//
//     auto response = messageHandler.handleMessage(validLoginMessage);
//
//     EXPECT_EQ(response.id, common::messages::MessageId::LoginResponse);
//     EXPECT_EQ(response.payload, tokenResponse);
// }
//
// TEST_F(MessageHandlerImplTest, handleLoginValidMessageLoginCommandThrowException)
//{
//     EXPECT_CALL(*loginUserCommandHandlerMock,
//     execute(validLoginUserCommandHandlerPayload)).WillOnce(Throw(exception));
//
//     auto response = messageHandler.handleMessage(validLoginMessage);
//
//     EXPECT_EQ(response.id, common::messages::MessageId::LoginResponse);
//     EXPECT_EQ(response.payload, exceptionResponse);
// }
//
// TEST_F(MessageHandlerImplTest, handleLoginMessageWithInvalidEmail)
//{
//     auto response = messageHandler.handleMessage(loginMessagedWithInvalidEmail);
//
//     EXPECT_EQ(response.id, common::messages::MessageId::LoginResponse);
//     EXPECT_EQ(response.payload, wrongEmailResponse);
// }
