cmake_minimum_required(VERSION 3.20)
project(fpvdeck_simulator VERSION 0.2.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_LIST_DIR}/lv_conf.h" CACHE PATH "" FORCE)
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_USE_THORVG_INTERNAL OFF CACHE BOOL "" FORCE)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
add_subdirectory("${REPO_ROOT}/managed_components/lvgl__lvgl" lvgl)
target_link_libraries(lvgl PUBLIC PkgConfig::SDL2 m)

set(APP_SOURCES
    "${REPO_ROOT}/firmware/main/app_launcher.c"
    "${REPO_ROOT}/firmware/main/app_battery.c"
    "${REPO_ROOT}/firmware/main/app_monitor.c"
    "${REPO_ROOT}/firmware/main/app_finder.c"
    "${REPO_ROOT}/firmware/main/app_stream.c"
    "${REPO_ROOT}/firmware/main/app_settings.c"
    "${REPO_ROOT}/firmware/main/battery.c"
    "${REPO_ROOT}/firmware/main/deck_power.c"
    "${REPO_ROOT}/firmware/main/rx5808.c"
    "${REPO_ROOT}/firmware/main/signal_logic.c"
    main.c sim_backend.c sim_panel.c sim_video.c settings_sim.c app_scanner_sim.c)

add_executable(fpvdeck-sim ${APP_SOURCES})
target_compile_definitions(fpvdeck-sim PRIVATE FPVDECK_SIMULATOR=1 SDL_MAIN_HANDLED=1)
target_include_directories(fpvdeck-sim PRIVATE
    "${REPO_ROOT}/firmware/main" "${CMAKE_CURRENT_LIST_DIR}")
target_link_libraries(fpvdeck-sim PRIVATE lvgl PkgConfig::SDL2 m)
target_compile_options(fpvdeck-sim PRIVATE -Wall -Wextra -Werror)

enable_testing()
foreach(SCREEN launcher battery monitor scanner finder stream settings)
    add_test(NAME simulator-${SCREEN}-smoke
             COMMAND fpvdeck-sim --screen ${SCREEN} --smoke-test)
    set_tests_properties(simulator-${SCREEN}-smoke PROPERTIES
        ENVIRONMENT "SDL_VIDEODRIVER=offscreen;SDL_AUDIODRIVER=dummy"
        TIMEOUT 15)
endforeach()
