include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.10
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(Catch)

add_executable(vertex_tests
    ../src/structs/vertex.cpp
    unit/vertex_test.cpp
)

target_include_directories(vertex_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(vertex_tests PRIVATE Catch2::Catch2)
catch_discover_tests(vertex_tests)

add_executable(graph_tests
    ../src/structs/vertex.cpp
    ../src/structs/graph.cpp
    unit/graph_test.cpp
)

target_include_directories(graph_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(graph_tests PRIVATE Catch2::Catch2)
catch_discover_tests(graph_tests)

add_executable(dynamic_connectivity_tests
    ../src/structs/vertex.cpp
    ../src/structs/graph.cpp
    ../src/algorithms/connectivity/dynamic_connectivity.cpp
    unit/dynamic_connectivity_test.cpp
)

target_include_directories(dynamic_connectivity_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(dynamic_connectivity_tests PRIVATE Catch2::Catch2)
catch_discover_tests(dynamic_connectivity_tests)

add_executable(dynamic_mst_tests
    ../src/structs/vertex.cpp
    ../src/structs/graph.cpp
    ../src/algorithms/mst/dynamic_mst_incremental.cpp
    unit/dynamic_mst_test.cpp
)

target_include_directories(dynamic_mst_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(dynamic_mst_tests PRIVATE Catch2::Catch2)
catch_discover_tests(dynamic_mst_tests)

add_executable(suitor_tests
    ../src/structs/vertex.cpp
    ../src/structs/graph.cpp
    ../src/algorithms/suitor/suitor_matching.cpp
    unit/suitor_test.cpp
)

target_include_directories(suitor_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(suitor_tests PRIVATE Catch2::Catch2)
catch_discover_tests(suitor_tests)
