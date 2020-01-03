#include "catch.hpp"
#include "shapes.hpp"

using namespace ls;

TEST_CASE( "Group processing", "[groups]" )
{
    SECTION( "Creating a new group" )
    {
    	auto g = group::create();

    	REQUIRE( g->transform() == f4_matrix::identity() );
    	REQUIRE( g->children().empty() );
    }

    SECTION( "Adding a child to a group" )
    {
        auto g = group::create();
        auto sh = sphere::create();
        g->add_child( sh );
        auto children = g->children();

        REQUIRE( !children.empty() );
        REQUIRE( std::find( children.begin(), children.end(), sh ) != children.end() );
        REQUIRE( sh->parent() == g );
    }
};
