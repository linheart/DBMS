cmake_minimum_required(VERSION 3.30)

project(DBMS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
add_link_options(-fsanitize=address -fno-omit-frame-pointer -g)

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

file(COPY ${CMAKE_SOURCE_DIR}/src/schema.json DESTINATION ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})
