add_executable(test_GtrMt test_GtrMt.c ${PROJECT_SOURCE_DIR}/third_party/unity/unity.c)

target_include_directories(test_GtrMt PRIVATE ${PROJECT_SOURCE_DIR}/third_party/unity)

target_link_libraries(test_GtrMt PRIVATE core)

add_test(NAME test_GtrMt COMMAND test_GtrMt)