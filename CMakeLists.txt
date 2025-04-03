cmake_minimum_required(VERSION 3.14)
project(LennardJonesMD VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add main library
add_library(ljmd_lib
    src/core/particle.cpp
    src/core/system.cpp
    src/core/box.cpp
    src/core/forces.cpp
    src/integrators/velocity_verlet.cpp
    src/utils/vector3d.cpp
    src/utils/random.cpp
    src/utils/config_reader.cpp
)

# Add main executable
add_executable(ljmd src/main.cpp)
target_link_libraries(ljmd PRIVATE ljmd_lib)

# Enable testing
enable_testing()
add_subdirectory(test)