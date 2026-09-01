add_library(fpvdeck_mcu_protocol STATIC
    src/board.c
    src/protocol.c
    include/fpvdeck/board.h
    include/fpvdeck/protocol.h
)
target_include_directories(fpvdeck_mcu_protocol PUBLIC include)
target_compile_features(fpvdeck_mcu_protocol PUBLIC c_std_11)
target_compile_options(fpvdeck_mcu_protocol PRIVATE -Wall -Wextra -Wpedantic)

add_executable(test_mcu_protocol tests/test_protocol.c)
target_link_libraries(test_mcu_protocol PRIVATE fpvdeck_mcu_protocol)
target_compile_options(test_mcu_protocol PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME test_mcu_protocol COMMAND test_mcu_protocol)

add_executable(test_mcu_board tests/test_board.c)
target_link_libraries(test_mcu_board PRIVATE fpvdeck_mcu_protocol)
target_compile_options(test_mcu_board PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME test_mcu_board COMMAND test_mcu_board)
