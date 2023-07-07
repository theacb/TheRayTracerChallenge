cmake_minimum_required(VERSION 3.25)
project(Raymond)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/be03d00f5f0cc3a997d1a368bee8a1fe93651f48.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
        Raymond
        Raymond/main.cpp
        Raymond/pch.cpp
        Raymond/Background.cpp
        Raymond/BoundingBox.cpp
        Raymond/Camera.cpp
        Raymond/Canvas.cpp
        Raymond/Color.cpp
        Raymond/IxComps.cpp
        Raymond/Light.cpp
        Raymond/Material.cpp
        Raymond/Matrix.cpp
        Raymond/Noise.cpp
        Raymond/Object.cpp
        Raymond/Primitive.cpp
        Raymond/PrimitiveDefinition.cpp
        Raymond/Quadtree.cpp
        Raymond/Ray.cpp
        Raymond/Sample.cpp
        Raymond/SampleBuffer.cpp
        Raymond/Texmap.cpp
        Raymond/Tuple.cpp
        Raymond/Utilities.cpp
        Raymond/World.cpp
)

add_executable(
        raymond_unit_tester
        RaymondUnitTester/test.cpp
        Raymond/pch.cpp
        Raymond/Background.cpp
        Raymond/BoundingBox.cpp
        Raymond/Camera.cpp
        Raymond/Canvas.cpp
        Raymond/Color.cpp
        Raymond/IxComps.cpp
        Raymond/Light.cpp
        Raymond/Material.cpp
        Raymond/Matrix.cpp
        Raymond/Noise.cpp
        Raymond/Object.cpp
        Raymond/Primitive.cpp
        Raymond/PrimitiveDefinition.cpp
        Raymond/Quadtree.cpp
        Raymond/Ray.cpp
        Raymond/Sample.cpp
        Raymond/SampleBuffer.cpp
        Raymond/Texmap.cpp
        Raymond/Tuple.cpp
        Raymond/Utilities.cpp
        Raymond/World.cpp
)

target_link_libraries(
        raymond_unit_tester
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(raymond_unit_tester)