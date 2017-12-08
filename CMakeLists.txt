cmake_minimum_required(VERSION 3.5.0)

#set(CMAKE_BUILD_TYPE "Release")

project(ProjectName)

include(ExternalProject)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

ExternalProject_Add(GLM
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master

  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  INSTALL_COMMAND ""
)
ExternalProject_Get_Property(GLM SOURCE_DIR)
include_directories(${SOURCE_DIR})


ExternalProject_Add(JSON
  GIT_REPOSITORY https://github.com/miloyip/rapidjson.git
  GIT_TAG master

  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  INSTALL_COMMAND ""

  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DRAPIDJSON_BUILD_DOC=OFF
             -DRAPIDJSON_BUILD_EXAMPLES=OFF
             -DRAPIDJSON_BUILD_TESTS=OFF
)
ExternalProject_Get_Property(JSON SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)


ExternalProject_Add(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG master

  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  INSTALL_COMMAND ""

  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DBUILD_SHARED_LIBS=OFF
)
ExternalProject_Get_Property(SFML SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)


include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(src)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "Jam")
