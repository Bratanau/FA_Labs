include_directories(include)

add_executable(task_24 
    src/main.c 
    src/cust_scanf.c)

target_link_libraries(task_24 m)
