include_directories(include)

add_executable(task_7 
    src/main.c
    src/number_utils.c)

target_link_libraries(task_7 m)


if(BUILD_TESTS)
    add_subdirectory(tests)
endif()