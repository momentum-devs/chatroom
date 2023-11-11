set(EXECUTABLE_NAME client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
        config/ConfigProvider.cpp
        api/SessionImpl.cpp
        api/SocketConnectorImpl.cpp
        api/MessageFactoryImpl.cpp
        gui/states/register/RegisterController.cpp
        gui/states/StateMachine.cpp
        gui/states/register/RegisterState.cpp
        gui/states/StateFactory.cpp
        gui/states/login/LoginState.cpp
        gui/states/login/LoginController.cpp
        gui/qml/LoaderController.cpp
        gui/states/createChannel/CreateChannelState.cpp
        gui/states/createChannel/CreateChannelController.cpp
        gui/states/sendFriendRequest/SendFriendRequestController.cpp
        gui/states/sendFriendRequest/SendFriendRequestState.cpp
        gui/states/userSettings/UserSettingsController.cpp
        gui/states/userSettings/UserSettingsState.cpp
        gui/states/verifyUser/VerifyUserState.cpp
        gui/states/verifyUser/VerifyUserController.cpp
        gui/states/inviteToChannel/InviteToChannelState.cpp
        gui/states/inviteToChannel/InviteToChannelController.cpp
        gui/controllers/LeftColumnController.cpp
        gui/states/privateMessages/PrivateMessagesState.cpp
        gui/states/privateMessages/PrivateMessagesController.h
        gui/states/privateMessages/PrivateMessagesController.cpp
        gui/states/channel/ChannelState.cpp
        gui/states/channel/ChannelController.cpp)

set(UT_SOURCES
        api/SessionImplTest.cpp
        gui/states/StateMachineTest.cpp
        api/MessageFactoryImplTest.cpp
        )

set(QML_SOURCES
        gui/qml/LoaderView.qml

        gui/qml/common/settings.js

        gui/qml/components/LeftColumn.qml
        gui/qml/components/ChannelList.qml
        gui/qml/components/ChannelInvitationList.qml
        gui/qml/components/SettingsBox.qml
        gui/qml/components/AddToListButton.ui.qml
        gui/qml/components/AcceptButton.ui.qml
        gui/qml/components/RejectButton.ui.qml
        gui/qml/components/Separator.ui.qml
        gui/qml/components/ActivityIndicator.ui.qml
        gui/qml/components/SettingsButton.ui.qml

        gui/states/register/RegisterView.qml
        gui/states/login/LoginView.qml
        gui/states/createChannel/CreateChannelView.qml
        gui/states/sendFriendRequest/SendFriendRequestView.qml
        gui/states/userSettings/UserSettingsView.qml
        gui/states/verifyUser/VerifyUserView.qml
        gui/states/inviteToChannel/InviteToChannelView.qml

        gui/states/privateMessages/PrivateMessagesView.qml
        gui/states/privateMessages/components/FriendsColumn.qml
        gui/states/privateMessages/components/FriendTopBar.qml

        gui/states/channel/ChannelView.qml
        gui/states/channel/components/ChannelTopBar.qml
        )

set(RESOURCES
        resources/svg/gear.svg
        resources/svg/plus.svg
        resources/svg/online.svg
        resources/svg/offline.svg
        resources/svg/crown.svg
        )

qt_add_executable(${EXECUTABLE_NAME} Main.cpp ${SOURCES})

qt_add_qml_module(${EXECUTABLE_NAME}
        URI ${PROJECT_NAME}
        VERSION 6.2
        QML_FILES
        ${QML_SOURCES}
        RESOURCES
        ${RESOURCES}
        )

target_link_libraries(${EXECUTABLE_NAME} PUBLIC
        Qt6::Core
        Qt::Gui
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Svg
        loguru::loguru
        nlohmann_json::nlohmann_json
        messages
        filesystem
        collection
        envParser
        )

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${GMOCK_INCLUDE_DIR} ${DOTENV_INCLUDE_DIR})

add_executable(${EXECUTABLE_NAME}UT ${UT_SOURCES} ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME}UT PUBLIC
        GTest::gmock_main
        Qt6::Core
        Qt::Gui
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Svg
        loguru::loguru
        nlohmann_json::nlohmann_json
        messages
        filesystem
        collection
        envParser
        )

target_include_directories(${EXECUTABLE_NAME}UT PUBLIC ${GMOCK_INCLUDE_DIR})

add_test(NAME ${EXECUTABLE_NAME}UT COMMAND ${EXECUTABLE_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_code_coverage(${EXECUTABLE_NAME}UT ALL)
