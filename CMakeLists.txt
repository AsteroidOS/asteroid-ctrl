cmake_minimum_required(VERSION 3.15)
project(asteroid-ctrl)
find_package(Qt6 COMPONENTS Bluetooth)
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Bluetooth REQUIRED)
endif()
set(CMAKE_AUTOMOC ON)
add_subdirectory(libasteroid)
add_executable(asteroid-ctrl main.cpp controller.cpp)
set_target_properties(asteroid-ctrl PROPERTIES AUTOMOC ON)
target_include_directories(asteroid-ctrl PRIVATE libasteroid/src)
target_compile_features(asteroid-ctrl PUBLIC cxx_std_17)
target_link_libraries(asteroid-ctrl PRIVATE Qt::Bluetooth asteroid)
install(TARGETS asteroid-ctrl)
