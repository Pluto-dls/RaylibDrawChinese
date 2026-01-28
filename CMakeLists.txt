cmake_minimum_required(VERSION 3.10)
project(RaylibDrawChinese)

set(CMAKE_C_STANDARD 11)

# 1. 创建一个接口库目标
add_library(stb INTERFACE)
# 2. 为这个目标指定头文件包含目录
target_include_directories(stb INTERFACE ${CMAKE_SOURCE_DIR}/include/stb-master)

# 搜索raylib库文件
find_library(RAYLIB_LIBRARY
        NAMES lib/raylib libraylib   # 库的可能名称
        PATHS lib/raylib/raylib  # 搜索路径
        NO_DEFAULT_PATH          # 只在指定路径搜索
)

# 搜索头文件路径
find_path(RAYLIB_INCLUDE_DIR
        NAMES raylib.h
        PATHS lib/raylib/raylib/include
        lib/raylib
        NO_DEFAULT_PATH
)

if(RAYLIB_LIBRARY AND RAYLIB_INCLUDE_DIR)
    message(STATUS "找到raylib库: ${RAYLIB_LIBRARY}")
    message(STATUS "找到raylib头文件: ${RAYLIB_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "未找到raylib库！")
endif()

file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/include DESTINATION ${CMAKE_BINARY_DIR})

add_executable(RaylibDrawChinese
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
        src/chinesetext.c
        src/chinesetext.h
)


# 包含头文件路径
target_include_directories(RaylibDrawChinese PRIVATE ${RAYLIB_INCLUDE_DIR})
target_include_directories(RaylibDrawChinese PRIVATE src)

# 链接库
target_link_libraries(RaylibDrawChinese PRIVATE
        ${RAYLIB_LIBRARY}
        stb
)


if(WIN32)
    target_link_libraries(RaylibDrawChinese PRIVATE winmm)
endif()