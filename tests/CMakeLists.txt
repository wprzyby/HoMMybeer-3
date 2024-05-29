include(GoogleTest)
add_executable(tests
    main.cc
    test_unit_block.cc
    test_unit_block_generator.cc
    test_unit_config.cc
    test_unit_integration.cc
)

target_link_libraries(tests GTest::gtest_main GTest::gmock_main game_logic)

gtest_discover_tests(tests)

