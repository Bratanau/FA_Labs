include_directories(include)

add_executable(task_1 src/main.c src/has_finite_representation.c)

target_link_libraries(task_1 m)
