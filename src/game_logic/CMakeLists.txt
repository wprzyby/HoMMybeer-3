add_library(game_logic STATIC Session.cc Game.cc Map.cc)

target_include_directories(game_logic PUBLIC .)