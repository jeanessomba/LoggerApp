cmake_minimum_required(VERSION 3.14)

project(LoggerApp LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(LoggerApp
  main.cpp
  LoggerClass.h
  LoggerClass.cpp
  TestClass.cpp
  TestClass.h
)
target_link_libraries(LoggerApp Qt${QT_VERSION_MAJOR}::Core)

install(TARGETS LoggerApp
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
