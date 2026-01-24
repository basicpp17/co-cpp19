include(FetchContent)
find_package(GTest QUIET)
find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

if(NOT ${GTest_FOUND})
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        CMAKE_ARGS -DINSTALL_GTEST:BOOL=OFF
    )
    FetchContent_MakeAvailable(googletest)
endif()
