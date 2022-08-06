cmake_minimum_required(VERSION 3.22)
project(CSVGraph)
set(CMAKE_CXX_STANDARD 20)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
add_executable(CSVGraph main.cpp
        lib/ImGui/imgui_impl_opengl3_loader.h
        lib/ImGui/imgui_impl_opengl3.h
        lib/ImGui/imgui_impl_glfw.h
        lib/ImGui/imgui_impl_opengl3.cpp
        lib/ImGui/imgui_impl_glfw.cpp
        lib/ImGui/imconfig.h
        lib/ImGui/imgui.h
        lib/ImGui/imgui.cpp
        lib/ImGui/imgui_demo.cpp
        lib/ImGui/imgui_draw.cpp
        lib/ImGui/imgui_internal.h
        lib/ImGui/imgui_widgets.cpp
        lib/ImGui/imstb_rectpack.h
        lib/ImGui/imgui_tables.cpp
        lib/ImGui/imstb_textedit.h
        lib/ImGui/imstb_truetype.h

        lib/ImGui/implot.h
        lib/ImGui/implot.cpp
        lib/ImGui/implot_demo.cpp
        lib/ImGui/implot_internal.h
        lib/ImGui/implot_items.cpp
        )
target_include_directories(
    CSVGraph PRIVATE
    ${GLEW_INCLUDE_DIRS}
    ${OPEN_GL_INCLUDE_DIR}
)
target_link_libraries(
    CSVGraph
    glfw3
    glew32
    opengl32
    ${GLEW_LIBRARIES}
    ${OPEN_GL_LIBRARIES}
)