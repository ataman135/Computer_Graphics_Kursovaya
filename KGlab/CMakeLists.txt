cmake_minimum_required(VERSION 3.11)
project(kg_2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  glfw
  URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

FILE (GLOB SOURCES "*.cpp")
FILE (GLOB HEADERS "*.h")

list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp")

add_executable(kg_lab ${SOURCES} ${HEADERS} "include/Vector3.h")

target_link_libraries(kg_lab PRIVATE 
    glfw 
    OpenGL::GL 
    ${OPENGL_glu_LIBRARY}
)

target_include_directories(kg_lab PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)