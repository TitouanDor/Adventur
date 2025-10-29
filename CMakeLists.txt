cmake_minimum_required(VERSION 3.10)

project(Adventur LANGUAGES C)

find_package(SDL3 REQUIRED)
find_package(json-c CONFIG REQUIRED)

add_executable(
    Adventur 
    main.c
    Player.c
    canva.c
    Log.c
    )

target_link_libraries(Adventur PRIVATE SDL3::SDL3)
target_link_libraries(Adventur PRIVATE json-c::json-c)
