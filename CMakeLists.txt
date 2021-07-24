cmake_minimum_required(VERSION 3.15)

project(SimpleBrakeSim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(PROJECT_SOURCES
        src/main.cpp
        src/brakevis.cpp
        src/brakevis.h
        src/brakevis.ui
        src/BrakeParams.h
        src/BrakeSim.cpp
        src/BrakeSim.h
        src/CarSim.h
        src/EnvParams.h
        fig/SimpBrake.png
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SimpleBrakeSim
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(SimpleBrakeSim
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(SimpleBrakeSim PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${OpenCV_LIBS})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SimpleBrakeSim)
endif()
