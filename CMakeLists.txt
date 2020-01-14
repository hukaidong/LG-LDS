cmake_minimum_required(VERSION 3.10)
add_definitions(-Wfatal-errors)
add_definitions(-ggdb -o0 -std=gnu++17)
add_definitions(-ggdb -o0 -std=gnu++17)


project(showgl C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJ_SYSDEP_LIB pthread)
set(PROJECT_LIB ${PROJECT_NAME}b)
file(GLOB_RECURSE SOURCES src/*)
list(REMOVE_ITEM SOURCES src/main.cpp)
file(COPY 
  assets/fonts assets/shdr
  DESTINATION ${PROJECT_BINARY_DIR})

add_definitions(-DPROJ_DIR="${PROJECT_BINARY_DIR}")
add_executable(${PROJECT_NAME} src/main.cpp)
add_library(${PROJECT_LIB} ${SOURCES}) 

include(FindPkgConfig)
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf)
pkg_search_module(CZMQ REQUIRED libczmq)

include_directories(#project-showgl
                    ${PROJECT_SOURCE_DIR}/include
                    ${CZMQ_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
                    ${SDL2_INCLUDE_DIRS}
                    ${SDL2TTF_INCLUDE_DIRS}
                    ${SDL2IMAGE_INCLUDE_DIRS}
                   )

target_link_libraries(${PROJECT_NAME} 
                      ${PROJECT_LIB}
                      ${PROJ_SYSDEP_LIB}
                      ${CZMQ_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${SDL2_LIBRARIES}
                      ${SDL2TTF_LIBRARIES}
                      ${SDL2IMAGE_LIBRARIES}
                     )
