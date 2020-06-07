cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

project(network)
find_package(Qt5 REQUIRED Widgets Network Multimedia)

foreach(EXECUTABLE sender receiver)
    #
    add_executable(${EXECUTABLE} WIN32 ${EXECUTABLE}.cpp ${MOC_WRAPPER}
        ${UIC_WRAPPER} #${EXECUTABLE}.h
        ${EXECUTABLE}_main.cpp) # ${EXECUTABLE}_dialog.ui)
    #
    target_link_libraries(${EXECUTABLE} Qt5::Widgets Qt5::Network Qt5::Multimedia)
    #
endforeach()
