cmake_minimum_required(VERSION 3.10)
project(GraphicHW)

set(CMAKE_CXX_STANDARD 17)

# =========================================
# GLAD (OpenGL loader)
# =========================================
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

# =========================================
# GLFW
# =========================================
add_subdirectory(external/glfw)

# =========================================
# ImGui
# =========================================
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
)

target_link_libraries(imgui PUBLIC
    glfw
    glad
)

# =========================================
# stb image loader (header-only)
# =========================================
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
)

# =========================================
# GLM (header-only math library)
# =========================================
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
)

# =========================================
# Collect Source Files (recursive scan)
# =========================================
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# =========================================
# Executable
# =========================================
add_executable(GraphicHW ${SRC_FILES})

# =========================================
# Include paths
# =========================================
target_include_directories(GraphicHW PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# =========================================
# Link libraries
# =========================================
find_package(OpenGL REQUIRED)

target_link_libraries(GraphicHW PRIVATE
    OpenGL::GL
    glad
    glfw
    imgui
    glm
    stb
)

# =========================================
# Asset Copy (shaders / textures / fonts)
# Corrected for TASK 8
#
# Copy to:
#   <build>/assets/<subfolder>
# matching runtime expectation: "./assets/textures"
# =========================================

set(ASSETS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_DST_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSETS_DST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ASSETS_SRC_DIR}/shaders
            ${ASSETS_DST_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ASSETS_SRC_DIR}/textures
            ${ASSETS_DST_DIR}/textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ASSETS_SRC_DIR}/fonts
            ${ASSETS_DST_DIR}/fonts
)

add_dependencies(GraphicHW copy_assets)
