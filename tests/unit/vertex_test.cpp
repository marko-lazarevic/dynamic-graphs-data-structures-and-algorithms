#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../../src/structs/vertex.hpp"
#include "../../src/structs/graph.hpp"

TEST_CASE("AddNeighbor stores id/weight and increments degree") {
	Vertex v(1);
	v.AddNeighbor(2, 5);

	REQUIRE(v.Degree() == 1);
	REQUIRE(v.IsNeighbor(2));
	REQUIRE(v.GetWeight(2) == 5);
}

TEST_CASE("ChangeWeight updates GetWeight") {
	Vertex v(1);
	v.AddNeighbor(2, 3);
	v.ChangeWeight(2, 7);

	REQUIRE(v.GetWeight(2) == 7);
}

TEST_CASE("RemoveNeighbor clears mapping and keeps remaining neighbor") {
	Vertex v(1);
	v.AddNeighbor(2, 1);
	v.AddNeighbor(3, 2);

	v.RemoveNeighbor(2);

	REQUIRE(!v.IsNeighbor(2));
	REQUIRE(v.Degree() == 1);
	REQUIRE(v.IsNeighbor(3));
	REQUIRE(v.GetWeight(3) == 2);
}

TEST_CASE("SortEdges orders neighbors and rebuilds hashes") {
	Vertex v(1);
	v.AddNeighbor(2, 4);
	v.AddNeighbor(3, 10);
	v.AddNeighbor(4, 1);

	v.SortEdges();

	auto it = v.NeighbourIterator();
	REQUIRE((it + 0)->vertex_id == 2);
	REQUIRE((it + 1)->vertex_id == 3);
	REQUIRE((it + 2)->vertex_id == 4);

	REQUIRE(v.IsNeighbor(2));
	REQUIRE(v.GetWeight(2) == 4);
	REQUIRE(v.IsNeighbor(3));
	REQUIRE(v.GetWeight(3) == 10);
	REQUIRE(v.IsNeighbor(4));
	REQUIRE(v.GetWeight(4) == 1);
}

TEST_CASE("Resize keeps neighbors reachable") {
	Vertex v(1);
	v.AddNeighbor(2, 1);
	v.AddNeighbor(3, 2);
	v.AddNeighbor(4, 1);

	REQUIRE(v.Beta() == 8);
	REQUIRE(v.IsNeighbor(2));
	REQUIRE(v.IsNeighbor(3));
	REQUIRE(v.GetWeight(2) == 1);
	REQUIRE(v.GetWeight(3) == 2);
	REQUIRE(v.GetWeight(4) == 1);
}

TEST_CASE("Duplicate insertion is ignored") {
	Vertex v(1);
	v.AddNeighbor(2, 1);
	v.AddNeighbor(2, 99);

	REQUIRE(v.Degree() == 1);
	REQUIRE(v.IsNeighbor(2));
	REQUIRE(v.GetWeight(2) == 1);
}

TEST_CASE("Neighbour iterators span exactly degree entries") {
	Vertex v(1);
	v.AddNeighbor(5, 1);
	v.AddNeighbor(7, 2);
	v.AddNeighbor(9, 3);

	int count = 0;
	for (auto it = v.NeighbourIterator(); it != v.NeighbourEndIterator(); ++it) {
		count++;
	}

	REQUIRE(count == v.Degree());
}

TEST_CASE("RemoveNeighbor can preserve order") {
	Vertex v(1);
	v.AddNeighbor(2, 1);
	v.AddNeighbor(3, 2);
	v.AddNeighbor(4, 3);

	v.RemoveNeighbor(3, true);

	REQUIRE(v.Degree() == 2);
	auto it = v.NeighbourIterator();
	REQUIRE((it + 0)->vertex_id == 2);
	REQUIRE((it + 1)->vertex_id == 4);
	REQUIRE(v.IsNeighbor(2));
	REQUIRE(v.IsNeighbor(4));
	REQUIRE(!v.IsNeighbor(3));
}

TEST_CASE("Lots of neighbors") {
    Vertex v(1);
    for (int i = 2; i < 20; i++) {
        v.AddNeighbor(i, i * 10);
    }

    REQUIRE(v.Degree() == 18);
    for (int i = 2; i < 20; i++) {
        REQUIRE(v.IsNeighbor(i));
        REQUIRE(v.GetWeight(i) == i * 10);
    }
}

TEST_CASE("RemoveVertex removes incoming and reindexes IDs") {
	Graph g;
	g.AddVertex(); // 0
	g.AddVertex(); // 1
	g.AddVertex(); // 2

	g.AddEdge(0, 1, 5);
	g.AddEdge(1, 2, 6);
	g.AddEdge(2, 1, 7);

	g.RemoveVertex(1);

	REQUIRE(g.VertexCount() == 2);
	REQUIRE(!g.AreNeighbors(0, 1));
	REQUIRE(!g.AreNeighbors(1, 0));
}