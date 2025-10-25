cmake_minimum_required(VERSION 3.10)

project(Adventur LANGUAGES C)

find_package(SDL3 REQUIRED)

add_executable(
    Adventur 
    main.c
    )

target_link_libraries(Adventur PRIVATE SDL3::SDL3)
