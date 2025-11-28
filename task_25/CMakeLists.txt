include_directories(include)

add_executable(task_5_run src/main.c src/functions.c)

target_link_options(task_5_run PRIVATE -Wl,--subsystem,console)

target_link_libraries(task_5_run PRIVATE m)
