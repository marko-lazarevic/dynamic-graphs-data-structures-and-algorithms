add_executable(benchmark
	../src/structs/vertex.cpp
	../src/structs/graph.cpp
	../src/algorithms/connectivity/dynamic_connectivity.cpp
	../src/algorithms/mst/dynamic_mst_incremental.cpp
	../src/algorithms/suitor/suitor_matching.cpp
	execute_benchmarks.cpp
)

target_include_directories(benchmark PRIVATE
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}
)
