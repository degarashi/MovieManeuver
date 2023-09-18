cmake_minimum_required(VERSION 3.5)

project(MovieManeuver VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SRC_DIR src)
aux_source_directory(src SOURCE)
set(PROJECT_SOURCES
    ${SOURCE}
    src/winop.hpp
    src/xi_padstate.hpp
    src/vector.hpp
    src/manip.hpp
    src/manip_udemy.hpp
    src/manip_youtube.hpp
    src/mainwindow.h
    src/inputmgr_base.hpp
    src/buttonstate.hpp
    src/aux_func.hpp
    src/triggerstate.hpp
    src/axisstate.hpp
    src/aux_enums.hpp
    src/mainwindow.ui
    src/dg_diag_xinput.hpp
    src/diag_xinput.ui
    src/virtual_key.hpp
    src/inputmgr_xinput.hpp
    src/diag_wiimote.h
    src/diag_wiimote.ui
    src/wii_padstate.hpp
    src/inputmgr_wii.hpp
    src/wii_enums.hpp
    src/wii_exception.hpp
    src/xi_enums.hpp
    src/manip_box.hpp
    src/manip_vlc.hpp
    src/axisstate_n.hpp
    src/keydiff.hpp
    src/manip_mgr.hpp
    src/input.hpp
)

set(WIIREMOTE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${WIIREMOTE_INCLUDE_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MovieManeuver
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MovieManeuver APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MovieManeuver SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MovieManeuver
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set(WIIREMOTE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/libwiiremote.lib)
target_link_libraries(
    MovieManeuver
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${WIIREMOTE_LIB}
    xinput
    Dwmapi
)

set_target_properties(MovieManeuver PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS MovieManeuver
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MovieManeuver)
endif()
