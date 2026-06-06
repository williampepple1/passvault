cmake_minimum_required(VERSION 3.16)
project(QAESEncryption)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_library(QAESEncryption STATIC
    qaesencryption.cpp
)

target_link_libraries(QAESEncryption PRIVATE Qt6::Core)
target_include_directories(QAESEncryption PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
