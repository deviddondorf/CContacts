cmake_minimum_required(VERSION 4.1)
project(C_Contacts C)

set(CMAKE_C_STANDARD 99)

find_package(SQLite3 REQUIRED)

add_executable(C_Contacts main.c)

if(SQLite3_FOUND)
    target_include_directories(C_Contacts PRIVATE ${SQLite3_INCLUDE_DIRS})
    target_link_libraries(C_Contacts PRIVATE ${SQLite3_LIBRARIES})
endif()