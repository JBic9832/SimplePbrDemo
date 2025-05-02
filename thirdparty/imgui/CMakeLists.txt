cmake_minimum_required(VERSION 3.29)
project(imgui)

add_library(imgui)
target_sources(imgui PRIVATE 
"${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_glfw.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_opengl3.cpp"
)

target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")


target_link_libraries(imgui PUBLIC glfw glad)