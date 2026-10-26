cmake_minimum_required(VERSION 3.16)
project(fpvdeck_host_tests C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_executable(fpvdeck_host_tests
    test_main.c
    ../../main/rx5808.c
    ../../main/battery.c
    ../../main/deck_power.c
    ../../main/signal_logic.c)
target_include_directories(fpvdeck_host_tests PRIVATE ../../main)

enable_testing()
add_test(NAME fpvdeck_host_tests COMMAND fpvdeck_host_tests)
