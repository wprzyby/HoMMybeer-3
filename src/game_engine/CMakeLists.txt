add_library(game_engine STATIC views/MapView.cc)
target_link_libraries(game_engine PUBLIC game_logic sfml-graphics)
target_include_directories(game_engine PUBLIC views)