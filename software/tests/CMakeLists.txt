function(fpvdeck_add_test name source)
    qt_add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE fpvdeck_core Qt6::Test Qt6::Sql)
    target_compile_options(${name} PRIVATE -Wall -Wextra -Wpedantic)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

fpvdeck_add_test(test_battery_math test_battery_math.cpp)
fpvdeck_add_test(test_frame_codec test_frame_codec.cpp)
fpvdeck_add_test(test_database test_database.cpp)
fpvdeck_add_test(test_services test_services.cpp)

add_test(NAME test_qml_smoke COMMAND fpvdeck --database :memory: --exit-after-ms 500)
set_tests_properties(test_qml_smoke PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    TIMEOUT 10
)

foreach(state home fpv fpv-controls fpv-lock battery media media-playback receiver flights diagnostics)
    add_test(NAME test_qml_${state}
        COMMAND fpvdeck --database :memory: --demo-state ${state} --exit-after-ms 300)
    set_tests_properties(test_qml_${state} PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
        TIMEOUT 10
    )
endforeach()

find_package(Python3 REQUIRED COMPONENTS Interpreter)
add_test(NAME test_balance_frontend_error
    COMMAND ${Python3_EXECUTABLE} -m unittest
        tools.test_balance_frontend_error
        tools.test_fpvdeck_hw_test
        tools.test_test_board_source)
set_tests_properties(test_balance_frontend_error PROPERTIES
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
