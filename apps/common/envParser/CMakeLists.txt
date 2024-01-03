set(LIBRARY_NAME envParser)

set(SOURCES
        EnvironmentParser.cpp
)

add_library(${LIBRARY_NAME} ${SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC)

target_link_libraries(${LIBRARY_NAME} PUBLIC
        fmt::fmt
)
