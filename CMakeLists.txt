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

set(GLIB_SOURCE src/glib/glib_clock/glib_clock.cpp
                src/glib/glib_color/glib_color.cpp
                src/glib/glib_events/glib_events.cpp
                src/glib/glib_primitives/glib_primitives.cpp
                src/glib/glib_render_target/glib_render_target.cpp
                src/glib/glib_render_texture/glib_render_texture.cpp
                src/glib/glib_render_window/glib_render_window.cpp
                src/glib/glib_texture/glib_texture.cpp
                src/glib/glib_tools/glib_tools.cpp
                src/glib/glib_text/glib_text.cpp
                src/glib/glib_font/glib_font.cpp
)

set(ECS_SOURCE src/components/i_component.cpp
               src/components/comp_velocity/comp_velocity.cpp
               src/components/comp_position/comp_position.cpp
               src/entities/entity.cpp)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)


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
include_directories(${header_dir_list})

add_library(glib STATIC ${GLIB_SOURCE})
add_library(ECS STATIC ${ECS_SOURCE})

add_executable(Revenge src/main.cpp)
add_executable(Test src/test/test.cpp)

target_link_libraries(glib PUBLIC sfml-graphics)
target_link_libraries(Revenge PUBLIC glib)
target_link_libraries(ECS PUBLIC glib)
target_link_libraries(Test PUBLIC ECS)
