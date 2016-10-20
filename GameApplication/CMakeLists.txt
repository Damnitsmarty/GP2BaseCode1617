cmake_minimum_required (VERSION 2.8)
project (GameApplication)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -l")

find_package(SDL2 REQUIRED)
find_package(TinyXML REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_TTF REQUIRED)

add_subdirectory(Utils)

add_library(GameApplication STATIC src/GameApplication.cpp src/Shader.cpp include/Common.h include/GameApplication.h include/Shader.h)

include_directories(GameApplication include/ Utils/include ${SDL2_INCLUDE_DIR} ${TINYXML_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
# create the program
target_link_libraries(GameApplication Utils ${SDL2_LIBRARY} ${TINYXML_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${CMAKE_DL_LIBS} ${SDL2IMAGE_LIBRARY} ${SDL2_TTF_LIBRARIES})
