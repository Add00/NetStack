cmake_minimum_required(VERSION 3.18.0)
project(netstack VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_subdirectory(examples)

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    # catch2
    find_package(catch QUIET)
    if (NOT catch_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            catch
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.3.2
        )
        FetchContent_MakeAvailable(catch)
    endif()

    add_subdirectory(tests)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
