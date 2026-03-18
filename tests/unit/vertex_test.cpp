#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../../src/structs/vertex.hpp"

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
	v.AddNeighbor(3, 2); // triggers resize at beta/2 threshold

	REQUIRE(v.Beta() == 8);
	REQUIRE(v.IsNeighbor(2));
	REQUIRE(v.IsNeighbor(3));
	REQUIRE(v.GetWeight(2) == 1);
	REQUIRE(v.GetWeight(3) == 2);
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