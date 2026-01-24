
add_executable(flags19-test)
file(GLOB flags19_test_sources "${CMAKE_CURRENT_LIST_DIR}/*.test.cpp")
target_sources(flags19-test
    PRIVATE ${flags19_test_sources}
)
target_link_libraries(flags19-test
    PRIVATE GTest::gtest_main
    PRIVATE CoCpp19::flags19
)
add_test(NAME flags19 COMMAND flags19-test)
