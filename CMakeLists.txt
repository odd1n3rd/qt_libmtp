
cmake_minimum_required(VERSION 3.16)

project(untitled3 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt6 REQUIRED COMPONENTS Core Widgets)


find_library(LIBMTP_LIBRARY NAMES mtp libmtp HINTS /usr/lib /usr/lib/x86_64-linux-gnu)
if(NOT LIBMTP_LIBRARY)
    message(FATAL_ERROR "libmtp not found. Please install libmtp-dev or specify the path to libmtp.")
else()
    message(STATUS "Found libmtp: ${LIBMTP_LIBRARY}")
endif()


set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mtpdevice.cpp
    mtpdevice.h
    mtpviewmodel.cpp
    mtpviewmodel.h
)


qt_add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
    ${LIBMTP_LIBRARY}
)


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)


install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
