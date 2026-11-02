add_library(core core/core.c)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)

add_library(hal hal/reg_manip.c)
target_include_directories(hal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/hal)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/hal)