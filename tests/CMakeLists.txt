include(GoogleTest)
add_executable(tests
    main.cc
)

target_link_libraries(tests GTest::gtest_main GTest::gmock_main game_logic)

gtest_discover_tests(tests)

