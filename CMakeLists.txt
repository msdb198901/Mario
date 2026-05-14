cmake_minimum_required(VERSION 3.10)


# 强制 x86 架构
if(MSVC)
    # Visual Studio: 要求生成器平台为 Win32
    if(NOT CMAKE_GENERATOR_PLATFORM)
        set(CMAKE_GENERATOR_PLATFORM "Win32" CACHE STRING "Platform" FORCE)
    elseif(NOT CMAKE_GENERATOR_PLATFORM STREQUAL "Win32")
        message(FATAL_ERROR "This project requires x86 (Win32) platform. Current: ${CMAKE_GENERATOR_PLATFORM}")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC/Clang: 添加 -m32 标志
    add_compile_options(-m32)
    add_link_options(-m32)
else()
    message(WARNING "Unknown compiler, x86 not guaranteed")
endif()

project(Mario)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 收集项目中的源文件
file(GLOB_RECURSE SRC_FILES
    "main.cpp"
    "Components/*.cpp"
    "Editor/*.cpp"
    "Windows/*.cpp"
    "Observers/*.cpp"
    "Physics2d/*.cpp"
    "Renderer/*.cpp"
    "Scenes/*.cpp"
    "Util/*.cpp"
)

# 添加 glad.c（根据实际路径调整）
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/src/glad.c")
    list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/src/glad.c")
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/glad.c")
    list(APPEND SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/glad.c")
else()
    message(FATAL_ERROR "glad.c not found in thirdparty/glad/")
endif()

# ================== Dear ImGui 配置开始 ==================
# 2. 设置 ImGui 目录
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui)

# 3. 添加 ImGui 核心源文件 (使用 .cpp 文件)
list(APPEND SRC_FILES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    # 添加 ImGui 平台和后端源文件
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
# ================== Dear ImGui 配置结束 ==================

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 包含头文件目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Components
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/Observers
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor
    ${CMAKE_CURRENT_SOURCE_DIR}/Scenes
    ${CMAKE_CURRENT_SOURCE_DIR}/Windows
    ${CMAKE_CURRENT_SOURCE_DIR}/Util
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb_image
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/json/single_include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/box2d-2.4.2/include
    # 4. 添加 ImGui 头文件路径
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

# 查找 glfw 库文件（glad 不再需要单独链接）
find_library(GLFW_LIBRARY
    NAMES glfw3 glfw glfw3.lib libglfw3.a box2d.lib
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/lib
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/box2d-2.4.2/lib
    NO_DEFAULT_PATH
)

if(NOT GLFW_LIBRARY)
    message(FATAL_ERROR "GLFW library not found in thirdparty/glfw/lib")
endif()

# 查找系统 OpenGL 库
find_package(OpenGL REQUIRED)

# 链接库（glad 通过源代码直接编译，无需额外链接）
target_link_libraries(${PROJECT_NAME}
    ${GLFW_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${BOX2D_LIBRARY}
)

# Windows 下可能需要 opengl32
if(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
endif()