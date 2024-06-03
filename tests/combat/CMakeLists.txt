add_executable(combat_tests 
    test_battle_manager.cc
    test_battleground.cc 
    test_battlestate.cc
)

target_link_libraries(combat_tests GTest::gtest_main combat)

gtest_discover_tests(combat_tests)