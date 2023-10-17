cmake_minimum_required(VERSION 3.16)
project(QtNetworkAuthSandboxing VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core NetworkAuth)

qt_add_executable(QtNetworkAuthSandboxing
    main.cpp
)

target_link_libraries(QtNetworkAuthSandboxing PRIVATE Qt6::Core Qt6::NetworkAuth)
