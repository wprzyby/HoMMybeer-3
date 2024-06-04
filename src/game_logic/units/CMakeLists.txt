add_library(units STATIC
    UnitBlock.cc
    UnitBlockGenerator.cc
    UnitConfig.cc
)
target_include_directories(units PUBLIC .)
target_include_directories(units PRIVATE ${CMAKE_SOURCE_DIR}/lib/nlohmann)