cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
project(co-cpp19 LANGUAGES CXX)

set(COCPP_MAIN_PROJECT OFF)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(COCPP_MAIN_PROJECT ON)
endif()

option(COCPP_BuildTests "Build the unit tests when BUILD_TESTING is enabled." ${COCPP_MAIN_PROJECT})
option(COCPP_Install "Install CMake targets during install step." ${COCPP_MAIN_PROJECT})

if(COCPP_BuildTests)
    enable_testing()
endif()

if(COCPP_Install)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)
    set(COCPP19_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/CoCpp19")
endif()

add_subdirectory(third_party)
add_subdirectory(src)

if(COCPP_Install)
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/CoCpp19Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/CoCpp19Config.cmake"
        INSTALL_DESTINATION ${COCPP19_CMAKE_CONFIG_DESTINATION}
    )
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/CoCpp19ConfigVersion.cmake"
        VERSION "1.0.0"
        COMPATIBILITY AnyNewerVersion
    )
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/CoCpp19Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/CoCpp19ConfigVersion.cmake"
        DESTINATION ${COCPP19_CMAKE_CONFIG_DESTINATION}
        COMPONENT cocpp19
    )
endif()
