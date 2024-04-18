add_library(game_logic STATIC
                              Game.cc  
                              Map.cc 
                              Player.cc 
                              Field.cc
                              Hero.cc
                              generators/MapUtils.cc)

target_include_directories(game_logic PUBLIC . generators)