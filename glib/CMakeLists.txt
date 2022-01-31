cmake_minimum_required(VERSION 3.16.3)

set(GLIB_SOURCE src/glib_clock/glib_clock.cpp
                src/glib_color/glib_color.cpp
                src/glib_events/glib_events.cpp
                src/glib_primitives/glib_primitives.cpp
                src/glib_render_target/glib_render_target.cpp
                src/glib_render_texture/glib_render_texture.cpp
                src/glib_render_window/glib_render_window.cpp
                src/glib_texture/glib_texture.cpp
                src/glib_tools/glib_tools.cpp
                src/glib_text/glib_text.cpp
                src/glib_font/glib_font.cpp
)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_library(glib STATIC ${GLIB_SOURCE})
target_link_libraries(glib PUBLIC sfml-graphics)

include_directories(glib include/)
