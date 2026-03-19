include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.10
)
FetchContent_MakeAvailable(Catch2)

add_executable(vertex_tests
    ../src/structs/vertex.cpp
    ../src/structs/graph.cpp
    unit/vertex_test.cpp
)

target_include_directories(vertex_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(vertex_tests PRIVATE Catch2::Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(Catch)
catch_discover_tests(vertex_tests)
