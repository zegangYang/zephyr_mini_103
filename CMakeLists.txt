# Find Zephyr. This also loads Zephyr's build system.
cmake_minimum_required(VERSION 3.13.1)

SET(BOARD "mini_103_board")
SET(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
message(current : ${BOARD_ROOT})

find_package(Zephyr)
project(my_zephyr_app)

# Add your source file to the "app" target. This must come after
# find_package(Zephyr) which defines the target.
target_sources(app PRIVATE src/main.cpp)