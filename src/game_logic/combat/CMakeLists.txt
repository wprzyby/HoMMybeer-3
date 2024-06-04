add_library(combat STATIC   
    Battleground.cc
    BattleManager.cc
    BattleState.cc
    combat_utils.cc
)
target_include_directories(combat PUBLIC .)
target_link_libraries(combat game_logic_base)