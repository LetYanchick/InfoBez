cmake_minimum_required(VERSION 3.16)
project(SecureTrial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SecureTrial
    main.cpp
    Application.cpp
    Cipher.cpp
    FileControl.cpp
)

target_compile_options(SecureTrial PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O2
)
