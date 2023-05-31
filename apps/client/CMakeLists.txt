set(EXECUTABLE_NAME client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
        api/SessionImpl.cpp
        gui/MainView.cpp
        )

set(QML
        gui/qml/main.qml)

set(UT_SOURCES
        api/SessionImplTest.cpp
        )

qt_add_executable(${EXECUTABLE_NAME} Main.cpp ${SOURCES})

qt_add_qml_module(${EXECUTABLE_NAME}
    URI ${PROJECT_NAME}
    VERSION 6.2
    QML_FILES
        ${QML}
)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC
        Qt6::Core
        Qt::Gui
        Qt6::Quick
        Qt6::QuickControls2
        loguru::loguru
        messages
        environment
        filesystem
        collection)

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${GMOCK_INCLUDE_DIR} ${DOTENV_INCLUDE_DIR} ..)

add_executable(${EXECUTABLE_NAME}UT ${UT_SOURCES} ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME}UT PUBLIC gtest gmock_main)

target_include_directories(${EXECUTABLE_NAME}UT PUBLIC ${GMOCK_INCLUDE_DIR})

add_test(NAME ${EXECUTABLE_NAME}UT COMMAND ${EXECUTABLE_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})