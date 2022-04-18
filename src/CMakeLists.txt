target_include_directories(gscunit PUBLIC .)
target_sources(gscunit PRIVATE gscunit.cpp)
get_target_property(SOURCES gscunit SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
