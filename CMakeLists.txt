cmake_minimum_required(VERSION 3.10)
project(image_processing C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(image_processing
        src/main.c
        src/bmp8.c
        src/bmp24.c
        src/equalization.c
)

target_link_libraries(image_processing m)