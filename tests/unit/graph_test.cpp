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

TEST_CASE("AddEdge respects Graph edges_sorted_by_weight flag") {
	// default (not sorted)
	dg::Graph g1;
	g1.AddVertex(); // 0
	g1.AddVertex(); // 1
	g1.AddVertex(); // 2

	g1.AddEdge(0, 1, 5);
	g1.AddEdge(0, 2, 1);

	auto it1 = g1.NeighbourIterator(0);
	REQUIRE((it1 + 0)->vertex_id == 1);
	REQUIRE((it1 + 1)->vertex_id == 2);

	// sorted by weight
	dg::Graph g2(true);
	g2.AddVertex(); // 0
	g2.AddVertex(); // 1
	g2.AddVertex(); // 2

	g2.AddEdge(0, 1, 5);
	g2.AddEdge(0, 2, 1);

	auto it2 = g2.NeighbourIterator(0);
	REQUIRE((it2 + 0)->vertex_id == 2);
	REQUIRE((it2 + 1)->vertex_id == 1);
}
