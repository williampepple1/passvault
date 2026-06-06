cmake_minimum_required(VERSION 3.16)
project(PassVault VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Add QAESEncryption
add_subdirectory(libs/QAESEncryption)

# Application sources
set(PROJECT_SOURCES
    src/main.cpp
    src/CryptoManager.cpp
    src/CryptoManager.h
    src/VaultManager.cpp
    src/VaultManager.h
    src/SetupWindow.cpp
    src/SetupWindow.h
    src/LoginWindow.cpp
    src/LoginWindow.h
    src/MainWindow.cpp
    src/MainWindow.h
    src/EntryDialog.cpp
    src/EntryDialog.h
    resources/resources.qrc
    app_icon.rc
)

add_executable(PassVault
    ${PROJECT_SOURCES}
)

target_link_libraries(PassVault PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets QAESEncryption)

# Windows specific
if(WIN32)
    set_target_properties(PassVault PROPERTIES WIN32_EXECUTABLE ON)
endif()
