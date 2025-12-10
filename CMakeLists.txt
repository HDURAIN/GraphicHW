cmake_minimum_required(VERSION 3.10)
project(GraphicHW)

set(CMAKE_CXX_STANDARD 17)

# ======================================================
#  BUILD A FULLY STATIC EXECUTABLE (NO DLL REQUIRED)
# ======================================================
set(BUILD_SHARED_LIBS OFF)        # force static libraries
set(GLFW_BUILD_SHARED_LIBS OFF)   # ensure GLFW builds static
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

# ========================
# GLAD (static)
# ========================
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# ========================
# GLFW (static)
# ========================
add_subdirectory(external/glfw)

# ========================
# ImGui (static)
# ========================
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
)

target_link_libraries(imgui PUBLIC glfw glad)

# ========================
# stb_image (header-only)
# ========================
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE external/stb)

# ========================
# glm (header-only)
# ========================
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external/glm)

# ========================
# Collect source files
# ========================
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(GraphicHW ${SRC_FILES})

# ========================
# Includes
# ========================
target_include_directories(GraphicHW PUBLIC
    src
    external
)

# ========================
# Link (STATIC EVERYTHING)
# ========================
find_package(OpenGL REQUIRED)

target_link_libraries(GraphicHW PRIVATE
    OpenGL::GL
    glad
    glfw
    imgui
    glm
    stb
)

# Force static linking of system runtime (optional)
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ========================
# Copy assets
# ========================
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
