idf_component_register(
    SRCS
        main.c rx5808.c tvp5150.c video.c ads1115.c battery.c deck_power.c signal_logic.c
        buzzer.c buttons.c settings.c app_launcher.c app_battery.c
        app_monitor.c app_scanner.c app_finder.c app_stream.c app_settings.c
    INCLUDE_DIRS .
    REQUIRES nvs_flash esp_driver_cam esp_driver_isp esp_driver_i2c
             esp_driver_gpio esp_driver_ledc esp_adc
)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Wextra -Werror)
