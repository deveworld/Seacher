cmake_minimum_required(VERSION 3.0.0)
project(Seacher VERSION 0.1.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

file(GLOB SOURCES src/*.cpp)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES}
)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
