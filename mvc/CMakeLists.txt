cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(mvc)
find_package(Qt5 REQUIRED Widgets LinguistTools)

# Translation

set(FILES_TO_TRANSLATE main.cpp main_win.cpp
    doc_window.cpp combo_delegate.cpp tree_item.cpp tree_model.cpp tree_view.cpp)

set(TS_FILES translations/mvc-04_en_RU.ts)
option(UPDATE_TRANSLATIONS "Update source translation (files translations/*.ts)." ON)

if(UPDATE_TRANSLATIONS)
    message(STATUS "UPDATE_TRANSLATIONS option is set.")
    qt5_create_translation(QM_FILES ${FILES_TO_TRANSLATE} ${TS_FILES})
else()
    qt5_add_translation(QM_FILES ${TS_FILES})
endif()

add_custom_target(translations DEPENDS ${QM_FILES})
set_property(DIRECTORY PROPERTY CLEAN_NO_CUSTOM TRUE)

# End Translation

qt5_wrap_cpp(MOC_WRAPPERS main_win.h doc_window.h tree_model.h tree_view.h)
qt5_add_resources(QRC_WRAPPERS mvc-04.qrc OPTIONS -no-compress)
qt5_wrap_ui(UIC_WRAPPERS main_window.ui)

add_executable(${PROJECT_NAME} WIN32 ${FILES_TO_TRANSLATE} ${MOC_WRAPPERS} ${QRC_WRAPPERS} ${UIC_WRAPPERS})

target_link_libraries(mvc Qt5::Widgets)

# Installation

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

install(FILES ${QM_FILES} DESTINATION translations)

get_property(LIB_QT5CORE_PATH TARGET Qt5::Core PROPERTY IMPORTED_LOCATION_RELEASE)

get_filename_component(QT_LIBRARY_DIR "${LIB_QT5CORE_PATH}" DIRECTORY)

set(EXE_PATH bin/${PROJECT_NAME} ${CMAKE_EXECUTABLE_SUFFIX})

include(DeployQt4)
install_qt4_executable(
    "${EXE_PATH}"
    ""
    ""
    "${QT_LIBRARY_DIR}")

