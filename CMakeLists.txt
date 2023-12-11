cmake_minimum_required(VERSION 3.15)

project(backgroundSubtraction)

set ( CMAKE_CXX_STANDARD          20 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )

file(GLOB SRC_FILES "src/*.cpp")

include_directories(src)

# # Find SDL2 package
# find_package(SDL2 REQUIRED)
#
# # Find OpenMP package
# find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Add compiler flags for OpenMP
if(OpenMP_CXX_FOUND)
    target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_CXX_FLAGS})
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

# # Add include directories for SDL2
# target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
#
# # Link SDL2 libraries
# target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
