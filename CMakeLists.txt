cmake_minimum_required(VERSION 3.16)
project(RecordManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(record_manager
    src/main.cpp
    src/models/Book.cpp
    src/repositories/BookRepository.cpp
    src/services/BookService.cpp
    src/ui/Menu.cpp
)

target_include_directories(record_manager PRIVATE include)

if (MSVC)
    target_compile_options(record_manager PRIVATE /W4 /permissive-)
else()
    target_compile_options(record_manager PRIVATE -Wall -Wextra -Wpedantic)
endif()
