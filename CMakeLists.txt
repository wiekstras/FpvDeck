cmake_minimum_required(VERSION 3.22)

project(FpvDeck VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(FPVDECK_BUILD_TESTS "Build FPVDeck tests" ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Gui Quick Qml Multimedia Sql)
qt_standard_project_setup()

add_subdirectory(software)

if(FPVDECK_BUILD_TESTS)
    enable_testing()
    find_package(Qt6 6.4 REQUIRED COMPONENTS Test)
    add_subdirectory(software/tests)
    add_subdirectory(firmware/controller)
endif()
