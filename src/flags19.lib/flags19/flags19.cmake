add_library(flags19 INTERFACE)
target_link_libraries(flags19
    INTERFACE CoCpp19::enum19
)
target_include_directories(flags19
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
file(GLOB flags19_headers "${CMAKE_CURRENT_LIST_DIR}/*.h")
target_sources(flags19
    INTERFACE FILE_SET public_headers
    TYPE HEADERS
    FILES ${flags19_headers}
)

add_library(CoCpp19::flags19 ALIAS flags19)

if(COCPP_Install)
    install(TARGETS flags19
        EXPORT flags19Targets
        FILE_SET public_headers
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT "flags19"
    )
    install(EXPORT flags19Targets
        FILE "CoCpp19-flags19-targets.cmake"
        NAMESPACE "CoCpp19::"
        DESTINATION ${COCPP19_CMAKE_CONFIG_DESTINATION}
        COMPONENT "flags19"
    )
endif()
