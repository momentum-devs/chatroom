set(EXECUTABLE_NAME management)

set(SOURCES
        DatabaseManagerFactory.cpp
        )

set(UT_SOURCES
        )

add_executable(${EXECUTABLE_NAME} Main.cpp ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        TinyOrm::TinyOrm
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        environment
        filesystem
        )

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${TOM_INCLUDE_DIR} ${DOTENV_INCLUDE_DIR})
