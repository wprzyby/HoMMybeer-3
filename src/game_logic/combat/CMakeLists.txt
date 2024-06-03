add_library(combat STATIC   
    Battleground.cc
    BattleManager.cc
    BattleState.cc
    common.cc
)
target_include_directories(combat PUBLIC .)
target_link_libraries(combat game_logic)