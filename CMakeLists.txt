cmake_minimum_required(VERSION 3.22)

# 设置项目名称
set(CMAKE_PROJECT_NAME study)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

#####################添加C++编译与版本配置#################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(SIZE arm-none-eabi-size)

# 定义构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# 启用编译命令导出（便于 clangd 等工具索引）
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Add color diagnostics
set(CMAKE_COLOR_DIAGNOSTICS ON)

# 核心项目设置
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# 启用 C 和 ASM 语言支持
enable_language(C CXX ASM)

# Remove warning
add_compile_options(-w)

# 创建可执行文件
add_executable(${CMAKE_PROJECT_NAME})

# 1. 先添加子目录 (定义 stm32cubemx 库)
add_subdirectory(cmake/stm32cubemx)

# 2. 再链接库 (注意：这里去掉了 PRIVATE)
target_link_libraries(${CMAKE_PROJECT_NAME} stm32cubemx)

##############以下为修改过的cmakelist#################
function(USER_ADD_DIR dir)
    file(GLOB USER_SOURCE
        ${dir}/*.h
        ${dir}/*.c
        ${dir}/*.hpp
        ${dir}/*.cpp
        ${dir}/*.cc
    )
    # target_sources 必须带关键字，这不会跟 target_link_libraries 冲突，保持原样即可
    target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${USER_SOURCE})
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${dir})
endfunction()

###############################在此处添加你的文件路径#########
USER_ADD_DIR(${CMAKE_SOURCE_DIR}/src/motor)
USER_ADD_DIR(${CMAKE_SOURCE_DIR}/src/encoder)
USER_ADD_DIR(${CMAKE_SOURCE_DIR}/src/oled)
USER_ADD_DIR(${CMAKE_SOURCE_DIR}/src/mpu6050)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMENT "Building hex & bin file...\nEXCUTABLE SIZE:"
    COMMAND ${SIZE} ${PROJECT_NAME}.elf
)
