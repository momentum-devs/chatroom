set(SOURCES
        HttpClientImpl.cpp
        HttpClientFactory.cpp
)

add_library(httpClient ${SOURCES})

target_include_directories(httpClient
        PRIVATE
        ${CPR_INCLUDE_DIR}
)

target_link_libraries(httpClient
        PUBLIC
        cpr::cpr
        collection
)
