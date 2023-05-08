find_package(Boost REQUIRED)

add_subdirectory(googletest)
add_subdirectory(fmt)
set(LOGURU_WITH_STREAMS 1)
set(LOGURU_THREADNAME_WIDTH 11)
set(LOGURU_FILENAME_WIDTH 30)
add_subdirectory(loguru)