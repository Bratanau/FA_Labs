include_directories(include)

add_executable(task_4 
    src/main.c 
    src/over_scanf.c)

target_link_libraries(task_4 m)
