add_library(game_engine_views STATIC
    BattlegroundView.cc
    BattleUnitsView.cc
    BorderView.cc
    ControlsView.cc
    HexGridView.cc
    HeroView.cc
    MapView.cc
    ObjectsView.cc
    PathView.cc
    ResourcesView.cc
    TownView.cc
)
target_include_directories(game_engine_views PUBLIC .)
target_link_libraries(game_engine_views PUBLIC game_logic_base combat sfml-graphics)