cmake_minimum_required(VERSION 3.0.0)
project(Seacher VERSION 0.1.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

file(GLOB SOURCES src/*.cpp)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES}
)

include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})
add_definitions(${SDL2_CFLAGS_OTHER})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
