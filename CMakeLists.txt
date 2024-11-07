cmake_minimum_required(VERSION 3.15)

project(
    backgroundSubtraction
    VERSION 1.0
    DESCRIPTION "Background subtraction using Mixture of Gaussians"
    LANGUAGES CXX
)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable IPO/LTO if available
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR)
if(IPO_SUPPORTED)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "IPO/LTO enabled")
else()
    message(STATUS "IPO/LTO not supported: ${IPO_ERROR}")
endif()

# Set optimization flags based on compiler
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")

    # Enable vectorization reports
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopt-info-vec-optimized")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Rpass=loop-vectorize")
    endif()
endif()

# Source files
file(GLOB SRC_FILES "src/*.cpp")
include_directories(src)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

# Create executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Set include directories
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries and set compile options
if(OpenMP_CXX_FOUND)
    target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_CXX_FLAGS})
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

# Enable unity build
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES UNITY_BUILD ON UNITY_BUILD_BATCH_SIZE 10
)

# Add compiler definitions
target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE "$<$<CONFIG:Release>:NDEBUG>"
)
