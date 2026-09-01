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
    services/InteractionService.cpp
    services/InteractionService.h
    services/MediaService.cpp
    services/MediaService.h
    services/StorageService.cpp
    services/StorageService.h
    services/SystemService.cpp
    services/SystemService.h
    services/TelemetryService.cpp
    services/TelemetryService.h
    services/VideoService.cpp
    services/VideoService.h
)

target_include_directories(fpvdeck_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fpvdeck_core PUBLIC Qt6::Core Qt6::Sql)
target_compile_options(fpvdeck_core PRIVATE -Wall -Wextra -Wpedantic)

qt_add_executable(fpvdeck app/main.cpp)

set_source_files_properties(ui/theme/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(fpvdeck
    URI FPVDeck
    VERSION 1.0
    QML_FILES
        ui/Main.qml
        ui/theme/Theme.qml
        ui/components/AppMenu.qml
        ui/components/AppTile.qml
        ui/components/BatteryWidget.qml
        ui/components/BootSplash.qml
        ui/components/CellVoltageRow.qml
        ui/components/MediaTile.qml
        ui/components/OverlayCard.qml
        ui/components/StatusPill.qml
        ui/components/StatusChip.qml
        ui/components/Toast.qml
        ui/components/TopBar.qml
        ui/components/TouchButton.qml
        ui/pages/HomePage.qml
        ui/pages/FpvPage.qml
        ui/pages/BatteryPage.qml
        ui/pages/MediaPage.qml
        ui/pages/ReceiverPage.qml
        ui/pages/FlightsPage.qml
        ui/pages/BatteriesPage.qml
        ui/pages/DvrPage.qml
        ui/pages/AircraftPage.qml
        ui/pages/DiagnosticsPage.qml
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
