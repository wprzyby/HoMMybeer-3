add_library(generators STATIC map_generation.cc)
target_include_directories(generators PUBLIC .)
target_link_libraries(generators game_logic_base)