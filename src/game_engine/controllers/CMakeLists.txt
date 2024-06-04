add_library(game_engine_controllers STATIC 
    MainWindowController.cc
    MapWindowController.cc
    TownWindowController.cc
    BattleWindowController.cc
    EventHandler.cc
)
target_include_directories(game_engine_controllers PUBLIC .)
target_link_libraries(game_engine_controllers PUBLIC game_engine_views game_logic_base combat ai generators sfml-graphics)