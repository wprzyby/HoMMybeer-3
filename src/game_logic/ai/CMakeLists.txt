add_library(ai STATIC 
    MiniMaxFighter.cc
    combat_evaluation.cc
)
target_include_directories(ai PUBLIC .)
target_link_libraries(ai combat)