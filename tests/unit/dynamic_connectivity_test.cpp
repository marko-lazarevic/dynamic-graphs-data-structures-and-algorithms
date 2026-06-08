#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../../src/algorithms/connectivity/dynamic_connectivity.hpp"

TEST_CASE("AddVertex initializes singleton connectivity sets") {
	dynamic_connectivity::DynamicConnectivityIncremental dc;

	REQUIRE(dc.VertexCount() == 0);
	REQUIRE(dc.AddVertex() == 0);
	REQUIRE(dc.AddVertex() == 1);
	REQUIRE(dc.AddVertex() == 2);
	REQUIRE(dc.VertexCount() == 3);

	REQUIRE(dc.IsConnected(0, 0));
	REQUIRE(dc.IsConnected(1, 1));
	REQUIRE_FALSE(dc.IsConnected(0, 1));
	REQUIRE_FALSE(dc.IsConnected(1, 2));
}

TEST_CASE("AddEdge merges components transitively") {
	dynamic_connectivity::DynamicConnectivityIncremental dc;
	for (int i = 0; i < 4; ++i) {
		dc.AddVertex();
	}

	dc.AddEdge(0, 1, 3);
	dc.AddEdge(1, 2, 4);

	REQUIRE(dc.IsConnected(0, 1));
	REQUIRE(dc.IsConnected(1, 2));
	REQUIRE(dc.IsConnected(0, 2));
	REQUIRE_FALSE(dc.IsConnected(0, 3));

	dc.AddEdge(2, 3, 5);

	REQUIRE(dc.IsConnected(0, 3));
}

TEST_CASE("Edge weights are delegated to the underlying graph") {
	dynamic_connectivity::DynamicConnectivityIncremental dc;
	dc.AddVertex();
	dc.AddVertex();

	dc.AddEdge(0, 1, 11);

	REQUIRE(dc.GetEdgeWeight(0, 1) == 11);

	dc.UpdateEdgeWeight(0, 1, 17);

	REQUIRE(dc.GetEdgeWeight(0, 1) == 17);
}
