include_directories(../include)

add_executable(test_task7 
    main_test.c
    ../src/number_utils.c)

target_link_libraries(test_task7 m)

add_test(NAME test_task7 COMMAND test_task7)
