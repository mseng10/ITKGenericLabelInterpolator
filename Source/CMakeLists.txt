cmake_minimum_required(VERSION 2.8)
project(GenericLabelInterpolation)
# For YouCompleteMe
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable("RotateLabels" "RotateLabels.cxx")
target_link_libraries("RotateLabels" ${ITK_LIBRARIES})
