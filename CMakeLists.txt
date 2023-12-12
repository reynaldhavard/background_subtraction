cmake_minimum_required(VERSION 3.15)

project(backgroundSubtraction)

set ( CMAKE_CXX_STANDARD          20 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )

file(GLOB SRC_FILES "src/*.cpp")

include_directories(src)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
