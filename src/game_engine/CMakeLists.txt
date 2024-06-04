add_library(game_engine STATIC 
                    views/MapView.cc 
                    views/HeroView.cc 
                    MapWindowController.cc 
                    views/ControlsView.cc 
                    views/BorderView.cc 
                    views/ResourcesView.cc 
                    views/ObjectsView.cc
                    EventHandler.cc
                    views/PathView.cc
                    MainWindowController.cc
                    views/TownView.cc
                    TownWindowController.cc
                    BattleWindowController.cc
                    views/BattlegroundView.cc
                    views/HexGridView.cc
                    views/BattleUnitsView.cc) 
target_link_libraries(game_engine PUBLIC game_logic sfml-graphics)
target_include_directories(game_engine PUBLIC . views)
                    
