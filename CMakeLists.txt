cmake_minimum_required(VERSION 3.27)
project(Mti680_Data_receiver)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

link_directories(${CMAKE_SOURCE_DIR}/libs)

add_executable(Mti680_Data_receiver exemple_d'utilisation.cpp
        MtiDataReader.cpp
        MtiDataReader.h
        MtiDataValues.cpp
        MtiDataValues.h
        main.cpp
)

target_link_libraries(Mti680_Data_receiver
    ${CMAKE_SOURCE_DIR}/libs/xstypes64.lib
    ${CMAKE_SOURCE_DIR}/libs/xsensdeviceapi64.lib
)
