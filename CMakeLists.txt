cmake_minimum_required(VERSION 3.16)

idf_component_register(
    SRCS "aw9364.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio
)