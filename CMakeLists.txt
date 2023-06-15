cmake_minimum_required(VERSION 3.5)

project(UDP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Qt6 package
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

# Set source files for the UDP library
set(${PROJECT_NAME}SERVER udpsender.h udpsender.cpp)
set(${PROJECT_NAME}CLIENT udpreceiver.h udpreceiver.cpp)
set(${PROJECT_NAME}MANAGER udpmanager.h udpmanager.cpp)
set(${${PROJECT_NAME}_SOURCES}_SOURCES udpdef.h udp.h precompile.h
    ${${PROJECT_NAME}CLIENT}
    ${${PROJECT_NAME}SERVER}
    ${${PROJECT_NAME}MANAGER})

# Create the UDP library target
add_library(${PROJECT_NAME} STATIC ${${${PROJECT_NAME}_SOURCES}_SOURCES})

# Precompiled Header(TODO: carreful with precompile headers. reduces compilation time and causes some problems.)
target_precompile_headers(${PROJECT_NAME} PRIVATE precompile.h)

# Link the UDP library with Qt${QT_VERSION_MAJOR}
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                           Qt${QT_VERSION_MAJOR}::Network)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_LIBRARY)
