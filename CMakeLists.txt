cmake_minimum_required(VERSION 3.16)
project(Lab4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(LAB2_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Lab_2/library/include)
set(MODEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/app_gui/include
    ${MODEL_PATH}
    ${LAB2_PATH}
)

set(GUI_SOURCES
    app_gui/src/main.cpp
    app_gui/src/MainWindow.cpp
    app_gui/src/LazySequenceWidget.cpp
    app_gui/src/StatisticsWidget.cpp
    app_gui/src/TestingWidget.cpp
    app_gui/src/LazySequenceController.cpp
    app_gui/src/StatisticsController.cpp
)

set(GUI_HEADERS
    app_gui/include/MainWindow.hpp
    app_gui/include/LazySequenceWidget.hpp
    app_gui/include/StatisticsWidget.hpp
    app_gui/include/TestingWidget.hpp
    app_gui/include/LazySequenceController.hpp
    app_gui/include/StatisticsController.hpp
)

file(GLOB MODEL_FILES
    "${MODEL_PATH}/*.hpp"
    "${MODEL_PATH}/*.tpp"
)

add_executable(${PROJECT_NAME}
    ${GUI_SOURCES}
    ${GUI_HEADERS}
    ${MODEL_FILES}
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
)

add_subdirectory(tests)