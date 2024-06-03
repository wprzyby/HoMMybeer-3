add_library(game_logic STATIC
                              Game.cc  
                              Map.cc 
                              Player.cc 
                              Field.cc
                              Hero.cc
                              generators/MapUtils.cc
                              Session.cc
                              MapObject.cc
                              Config.cc
                              UnitBlock.cc
                              UnitBlockGenerator.cc
                              UnitConfig.cc
                              common.cc
)
target_include_directories(game_logic PUBLIC . generators ${CMAKE_SOURCE_DIR}/lib/nlohmann)
add_subdirectory(combat)