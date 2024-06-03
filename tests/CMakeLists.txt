include(GoogleTest)
add_subdirectory(combat)
add_executable(tests
    test_unit_block.cc
    test_unit_block_generator.cc
    test_unit_config.cc
    test_unit_integration.cc
    test_mapobject.cc
    test_hero_movement.cc
    test_session.cc
    test_game.cc
    test_map.cc
    testUtils.cc
)
target_include_directories(tests PRIVATE .)
target_link_libraries(tests GTest::gtest_main GTest::gmock_main game_logic)

gtest_discover_tests(tests)

