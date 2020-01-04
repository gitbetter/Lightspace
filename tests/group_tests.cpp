#include "catch.hpp"
#include "shapes.hpp"
#include "transform.hpp"

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

    SECTION( "Intersecting a ray with an empty group" )
    {
        auto g = group::create();
        auto r = ray( f_point( 0, 0, 0 ), f_vector( 0, 0, 1 ) );
        auto itrs = intersect( g, r );

        REQUIRE( itrs.size() == 0 );
    }

    SECTION( "Intersecting a ray with a non-empty group" )
    {
        auto g = group::create();
        auto s1 = sphere::create();
        auto s2 = sphere::create();
        s2->set_transform( transform::translation( 0.f, 0.f, -3.f ) );
        auto s3 = sphere::create();
        s3->set_transform( transform::translation( 5.f, 0.f, 0.f ) );
        g->add_child( s1 );
        g->add_child( s2 );
        g->add_child( s3 );
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto itrs = intersect( g, r );

        REQUIRE( itrs.size() == 4 );
        REQUIRE( itrs[0].object() == s2 );
        REQUIRE( itrs[1].object() == s2 );
        REQUIRE( itrs[2].object() == s1 );
        REQUIRE( itrs[3].object() == s1 );
    }

    SECTION( "Intersecting a transformed group" )
    {
        auto g = group::create();
        g->set_transform( transform::scale( 2.f, 2.f, 2.f ) );
        auto s = sphere::create();
        s->set_transform( transform::translation( 5.f, 0.f, 0.f ) );
        g->add_child( s );
        auto r = ray( f_point( 10, 0, -10 ), f_vector( 0, 0, 1 ) );
        auto itrs = intersect( g, r );

        REQUIRE( itrs.size() == 2 );
    }

    SECTION( "Converting a point from world to object space" )
    {
        auto g1 = group::create();
        g1->set_transform( transform::rotation_y( pi_over_2 ) );
        auto g2 = group::create();
        g2->set_transform( transform::scale( 2.f, 2.f, 2.f ) );
        g1->add_child( g2 );
        auto s = sphere::create();
        s->set_transform( transform::translation( 5.f, 0.f, 0.f ) );
        g2->add_child( s );
        auto p = s->world_to_object( f_point( -2, 0, -10 ) );

        REQUIRE( p == f_point( 0, 0, -1 ) );
    }

    SECTION( "Converting a normal from object to world space" )
    {
        auto g1 = group::create();
        g1->set_transform( transform::rotation_y( pi_over_2 ) );
        auto g2 = group::create();
        g2->set_transform( transform::scale( 1.f, 2.f, 3.f ) );
        g1->add_child( g2 );
        auto s = sphere::create();
        s->set_transform( transform::translation( 5.f, 0.f, 0.f ) );
        g2->add_child( s );
        auto n = s->normal_to_world( f_vector( 0.5773502f, 0.5773502f, 0.5773502f ) );

        REQUIRE( n == f_vector( 0.28571f, 0.42857f, -0.85714f ) );
    }

    SECTION( "Finding the normal on a child object" )
    {
        auto g1 = group::create();
        g1->set_transform( transform::rotation_y( pi_over_2 ) );
        auto g2 = group::create();
        g2->set_transform( transform::scale( 1.f, 2.f, 3.f ) );
        g1->add_child( g2 );
        auto s = sphere::create();
        s->set_transform( transform::translation( 5.f, 0.f, 0.f ) );
        g2->add_child( s );
        auto n = s->normal( 1.7321f, 1.1547f, -5.5774f );

        REQUIRE( n == f_vector( 0.28570f, 0.42854f, -0.85716f ) );
    }
};
