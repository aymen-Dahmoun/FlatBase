cmake_minimum_required(VERSION 3.10)
project(Gtk4App C)

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Find GTK4 package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# Include GTK4 headers and link libraries
include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})

# Add executable
add_executable(Gtk4App main.c
        functions.c
        main2.c
        functions.c)

# Link GTK4 libraries
target_link_libraries(Gtk4App ${GTK4_LIBRARIES})
