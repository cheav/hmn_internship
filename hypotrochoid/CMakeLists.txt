cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project(hypotrochoid)
find_package(Qt5Widgets)

qt5_wrap_cpp(MOC_WRAPPERS main_window.h input_win.h)

add_executable(${PROJECT_NAME} WIN32 main.cpp main_window.cpp input_win.cpp ${MOC_WRAPPERS})

target_link_libraries(hypotrochoid Qt5::Widgets)
