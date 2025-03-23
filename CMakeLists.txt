cmake_minimum_required(VERSION 3.16)

project(demo LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core SerialPort Network)

include_directories(${CMAKE_SOURCE_DIR}/DobotDll)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES
    main.cpp
    src/*.cpp
)

file(GLOB HEADERS
    include/*.h
)

add_executable(demo
    ${SOURCES}
    ${HEADERS}
)

# Link Qt libraries
target_link_libraries(demo
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::Network
)
target_link_libraries(demo
    ${CMAKE_SOURCE_DIR}/DobotDll/DobotDll.lib
)

add_custom_command(TARGET demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/DobotDll/DobotDll.dll
    $<TARGET_FILE_DIR:demo>
)


# Install rules
include(GNUInstallDirs)
install(TARGETS demo
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

