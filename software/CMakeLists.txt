add_library(fpvdeck_core STATIC
    core/BatteryMath.cpp
    core/BatteryMath.h
    core/FrameCodec.cpp
    core/FrameCodec.h
    services/BatteryService.cpp
    services/BatteryService.h
    services/DatabaseService.cpp
    services/DatabaseService.h
    services/DvrService.cpp
    services/DvrService.h
    services/InputService.cpp
    services/InputService.h
    services/TelemetryService.cpp
    services/TelemetryService.h
    services/VideoService.cpp
    services/VideoService.h
)

target_include_directories(fpvdeck_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fpvdeck_core PUBLIC Qt6::Core Qt6::Sql)
target_compile_options(fpvdeck_core PRIVATE -Wall -Wextra -Wpedantic)

qt_add_executable(fpvdeck app/main.cpp)

qt_add_qml_module(fpvdeck
    URI FPVDeck
    VERSION 1.0
    QML_FILES
        ui/Main.qml
        ui/components/AppMenu.qml
        ui/components/BatteryWidget.qml
        ui/components/StatusPill.qml
        ui/pages/FpvPage.qml
        ui/pages/BatteryPage.qml
        ui/pages/FlightsPage.qml
        ui/pages/DvrPage.qml
        ui/pages/AircraftPage.qml
        ui/pages/SettingsPage.qml
        ui/pages/SimulatorPage.qml
)

target_link_libraries(fpvdeck PRIVATE
    fpvdeck_core
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::Multimedia
    Qt6::Sql
)
target_compile_options(fpvdeck PRIVATE -Wall -Wextra -Wpedantic)

install(TARGETS fpvdeck RUNTIME DESTINATION bin)
