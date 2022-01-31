cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project(Repin VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(
  -O2
  -g

  #-fsanitize=address

  -Werror
  -Wall
  -Wextra
  -Wpedantic
)

add_link_options(
  #-fsanitize=address
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ECS_SOURCE src/ECS/components/i_component.cpp
               src/ECS/components/comp_velocity.cpp
               src/ECS/components/comp_position.cpp
               src/ECS/entities/entity.cpp)

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.hpp)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()


HEADER_DIRECTORIES(header_dir_list)
list(LENGTH header_dir_list header_dir_list_count)

add_subdirectory(glib/)

include_directories(${header_dir_list})


add_library(ECS STATIC ${ECS_SOURCE})

add_executable(Revenge src/main.cpp)
add_executable(Test src/test/test.cpp)

target_link_libraries(ECS PUBLIC glib)
target_link_libraries(Test PUBLIC ECS)
target_link_libraries(Revenge PUBLIC ECS)
