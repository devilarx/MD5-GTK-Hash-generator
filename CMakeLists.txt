cmake_minimum_required(VERSION 3.20)
project(GTKMD5 C)

set(CMAKE_C_STANDARD 11)

add_executable(GTKMD5 main.c md5.c md5.h )

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK4 REQUIRED IMPORTED_TARGET gtk4)

target_link_libraries(GTKMD5 PRIVATE PkgConfig::GTK4 m)
