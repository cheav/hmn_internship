cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(circles)
find_package(Qt5Widgets)
find_package(OpenCV REQUIRED imgproc) # PATHS ${OpenCV_PREFIX})

qt5_wrap_cpp(MOC_WRAPPERS main_window.h worker_thread.h asmOpenCV.h)
qt5_wrap_ui(UIC_WRAPPERS main_window.ui)

add_executable(${PROJECT_NAME} WIN32 main.cpp main_window.cpp worker_thread.cpp
    ${MOC_WRAPPERS} ${UIC_WRAPPERS})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${OpenCV_LIBS})
