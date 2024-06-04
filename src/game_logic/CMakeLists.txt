add_subdirectory(units)
add_library(game_logic_base STATIC
                              Config.cc
                              Field.cc
                              game_logic_utils.cc
                              Game.cc
                              Hero.cc
                              Map.cc
                              MapObject.cc
                              Player.cc
                              Session.cc
)
target_include_directories(game_logic_base PUBLIC . ${CMAKE_SOURCE_DIR}/lib/nlohmann)
target_link_libraries(game_logic_base units)
add_subdirectory(ai)
add_subdirectory(combat)
add_subdirectory(generators)