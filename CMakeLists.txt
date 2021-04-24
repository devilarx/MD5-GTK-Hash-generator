cmake_minimum_required(VERSION 3.20)
project(NuklearMD5 C)

set(CMAKE_C_STANDARD 11)

add_executable(NuklearMD5 main.c md5.c md5.h )
target_link_libraries(NuklearMD5 m)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
add_definitions (${GTK3_CFLAGS_OTHER})

include_directories (${GTK3_INCLUDE_DIRS})
link_directories (${GTK3_LIBRARY_DIRS})
target_link_libraries(NuklearMD5 ${GTK3_LIBRARIES})