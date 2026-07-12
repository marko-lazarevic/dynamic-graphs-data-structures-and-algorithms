#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../../src/algorithms/mst/dynamic_mst_incremental.hpp"

TEST_CASE("AddVertex appends vertices and starts with empty MST") {
	dynamic_mst::DynamicMSTIncremental mst;

	REQUIRE(mst.VertexCount() == 0);
	REQUIRE(mst.GetMSTWeight() == 0);

	REQUIRE(mst.AddVertex() == 0);
	REQUIRE(mst.AddVertex() == 1);
	REQUIRE(mst.AddVertex() == 2);
	REQUIRE(mst.VertexCount() == 3);

	REQUIRE(mst.GetMSTWeight() == 0);
	REQUIRE_FALSE(mst.IsInMST(0, 1));
}

TEST_CASE("AddEdge builds the expected MST for a weighted graph") {
	dynamic_mst::DynamicMSTIncremental mst;
	for (int i = 0; i < 4; ++i) {
		mst.AddVertex();
	}

	mst.AddEdge(0, 1, 4);
	mst.AddEdge(1, 2, 1);
	mst.AddEdge(0, 2, 3);
	mst.AddEdge(2, 3, 2);

	REQUIRE(mst.GetEdgeWeight(0, 1) == 4);
	REQUIRE(mst.GetEdgeWeight(1, 2) == 1);
	REQUIRE(mst.GetEdgeWeight(0, 2) == 3);
	REQUIRE(mst.GetEdgeWeight(2, 3) == 2);

	REQUIRE(mst.GetMSTWeight() == 6);
	REQUIRE(mst.IsInMST(1, 2));
	REQUIRE(mst.IsInMST(2, 3));
	REQUIRE(mst.IsInMST(0, 2));
	REQUIRE_FALSE(mst.IsInMST(0, 1));
}

TEST_CASE("UpdateEdgeWeight recomputes the MST when a better edge appears") {
	dynamic_mst::DynamicMSTIncremental mst;
	for (int i = 0; i < 3; ++i) {
		mst.AddVertex();
	}

	mst.AddEdge(0, 1, 10);
	mst.AddEdge(1, 2, 5);
	mst.AddEdge(0, 2, 6);

	REQUIRE(mst.GetMSTWeight() == 11);
	REQUIRE(mst.IsInMST(1, 2));
	REQUIRE(mst.IsInMST(0, 2));
	REQUIRE_FALSE(mst.IsInMST(0, 1));

	mst.UpdateEdgeWeight(0, 1, 1);

	REQUIRE(mst.GetEdgeWeight(0, 1) == 1);
	REQUIRE(mst.GetMSTWeight() == 6);
	REQUIRE(mst.IsInMST(0, 1));
	REQUIRE(mst.IsInMST(1, 2));
	REQUIRE_FALSE(mst.IsInMST(0, 2));
}
