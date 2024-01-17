// #include "gtest/gtest.h"
//
// #include "faker-cxx/Internet.h"
// #include "faker-cxx/String.h"
// #include "server/application/errors/ResourceNotFoundError.h"
// #include "server/application/services/hashService/HashServiceImpl.h"
// #include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
// #include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
// #include "server/tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.h"
// #include "server/tests/utils/userTestUtils/UserTestUtils.h"
// #include "UploadUserAvatarCommandHandlerImpl.h"
//
// using namespace ::testing;
// using namespace server;
// using namespace server::infrastructure;
// using namespace server::application;
// using namespace server::domain;
// using namespace server::tests;
//
// class UploadUserAvatarCommandImplIntegrationTest : public Test
//{
// public:
//     void SetUp() override
//     {
//         userTestUtils.truncateTable();
//     }
//
//     void TearDown() override
//     {
//         userTestUtils.truncateTable();
//     }
//
//     std::shared_ptr<odb::sqlite::database> db = DatabaseClientTestFactory::create();
//
//     UserTestUtils userTestUtils{db};
//
//     std::shared_ptr<UserMapper> userMapper = std::make_shared<UserMapperImpl>();
//
//     std::shared_ptr<UserRepository> userRepository = std::make_shared<UserRepositoryImpl>(db, userMapper);
//
//     std::shared_ptr<HashServiceImpl> hashService = std::make_shared<HashServiceImpl>();
//
//     UploadUserAvatarCommandHandlerImpl uploadUserAvatarCommandHandler{userRepository, hashService};
// };

// TEST_F(UploadUserAvatarCommandImplIntegrationTest, updateNotExistingUser_shouldThrow)
//{
//     const auto id = faker::String::uuid();
//     const auto password = faker::Internet::password();
//
//     ASSERT_THROW(uploadUserAvatarCommandHandler.execute({id, std::nullopt, password}),
//     errors::ResourceNotFoundError);
// }
//
// TEST_F(UploadUserAvatarCommandImplIntegrationTest, updatePassword)
//{
//     const auto user = userTestUtils.createAndPersist();
//
//     const auto updatedPassword = faker::Internet::password();
//
//     const auto [updatedUser] = uploadUserAvatarCommandHandler.execute({user->getId(), std::nullopt,
//     updatedPassword});
//
//     ASSERT_EQ(updatedUser.getId(), user->getId());
//     ASSERT_EQ(updatedUser.getPassword(), hashService->hash(updatedPassword));
// }
//
// TEST_F(UploadUserAvatarCommandImplIntegrationTest, updateNickname)
//{
//     const auto user = userTestUtils.createAndPersist();
//
//     const auto updatedNickname = faker::Internet::username();
//
//     const auto [updatedUser] = uploadUserAvatarCommandHandler.execute({user->getId(), updatedNickname,
//     std::nullopt});
//
//     ASSERT_EQ(updatedUser.getId(), user->getId());
//     ASSERT_EQ(updatedUser.getNickname(), updatedNickname);
// }
