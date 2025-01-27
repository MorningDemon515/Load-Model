cmake_minimum_required(VERSION 3.10)

project(Game)

add_executable(Game
        ${CMAKE_CURRENT_SOURCE_DIR}/glad4.6/src/glad.c
        Shader.cpp
        Camera.cpp
        #LoadModel.cpp
        Main.cpp
)

target_include_directories(Game PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include
        ${CMAKE_CURRENT_SOURCE_DIR}/glad4.6/include
        ${CMAKE_CURRENT_SOURCE_DIR}/glm
       ${CMAKE_CURRENT_SOURCE_DIR}/assimp-5.4.3/include
)

target_link_libraries(Game
        ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/libSDL2.dll.a
        ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/libSDL2_test.a
        ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/libSDL2main.a
        ${CMAKE_CURRENT_SOURCE_DIR}/assimp-5.4.3/build/lib/libassimp.dll.a
)