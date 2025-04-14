# Download and build googletest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add tests
add_executable(vector_test vector_test.cpp)
target_link_libraries(vector_test PRIVATE ljmd_lib gtest gtest_main)

add_executable(particle_test particle_test.cpp)
target_link_libraries(particle_test PRIVATE ljmd_lib gtest gtest_main)

# Add minimizer test
add_executable(minimizer_test minimizer_test.cpp)
target_link_libraries(minimizer_test PRIVATE ljmd_lib gtest gtest_main)

# Register tests
add_test(NAME VectorTest COMMAND vector_test)
add_test(NAME ParticleTest COMMAND particle_test)
add_test(NAME MinimizerTest COMMAND minimizer_test)