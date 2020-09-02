cmake_minimum_required(VERSION 3.16)

project(Restaurant_App LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Widgets REQUIRED)


add_executable(Restaurant_App
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    table.cpp
    table.h
    item.cpp
    item.h
    receipt.cpp
    receipt.h
    delegate.cpp 
    delegate.h
)


target_link_libraries(Restaurant_App PRIVATE Qt5::Widgets)
