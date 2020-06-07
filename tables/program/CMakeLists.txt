# CMakeLists.txt for program

add_executable(program main.cpp cout.cpp cout.h)

include_directories(../lib)

target_link_libraries(program lib)

# End file