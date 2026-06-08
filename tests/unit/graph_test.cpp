#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../../src/structs/graph.hpp"

TEST_CASE("AddVertex appends vertices and returns consecutive ids") {
	dg::Graph g;

	REQUIRE(g.VertexCount() == 0);
	REQUIRE(g.AddVertex() == 0);
	REQUIRE(g.AddVertex() == 1);
	REQUIRE(g.AddVertex() == 2);
	REQUIRE(g.VertexCount() == 3);

	auto it = g.VertexIterator();
	REQUIRE(it->Id() == 0);
	REQUIRE((it + 1)->Id() == 1);
	REQUIRE((it + 2)->Id() == 2);
}

TEST_CASE("AddEdge stores weights and updates directed adjacency") {
	dg::Graph g;
	g.AddVertex();
	g.AddVertex();

	g.AddEdge(0, 1, 5);

	REQUIRE(g.AreNeighbors(0, 1));
	REQUIRE(!g.AreNeighbors(1, 0));
	REQUIRE(g.GetEdgeWeight(0, 1) == 5);

	g.UpdateEdgeWeight(0, 1, 9);

	REQUIRE(g.GetEdgeWeight(0, 1) == 9);

	g.RemoveEdge(0, 1);

	REQUIRE(!g.AreNeighbors(0, 1));
}

TEST_CASE("RemoveVertex deletes incident edges and reindexes remaining vertices") {
	dg::Graph g;
	g.AddVertex(); // 0
	g.AddVertex(); // 1
	g.AddVertex(); // 2

	g.AddEdge(0, 1, 5);
	g.AddEdge(1, 2, 6);
	g.AddEdge(2, 1, 7);

	g.RemoveVertex(1);

	REQUIRE(g.VertexCount() == 2);

	auto it = g.VertexIterator();
	REQUIRE(it->Id() == 0);
	REQUIRE((it + 1)->Id() == 1);

	REQUIRE(!g.AreNeighbors(0, 1));
	REQUIRE(!g.AreNeighbors(1, 0));
	REQUIRE(g.NeighbourIterator(0) == g.NeighbourEndIterator(0));
}
