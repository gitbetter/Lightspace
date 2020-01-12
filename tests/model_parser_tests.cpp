#include "catch.hpp"
#include "shapes.hpp"
#include "model_parser.hpp"

using namespace ls;

TEST_CASE( "Model parser processing", "[model parser]" )
{
    SECTION( "Ignoring unrecognized lines" )
    {
        std::ifstream gibberish( "gibberish.obj" );
        auto parser = model_parser::obj( gibberish );

        REQUIRE( parser.error == nullptr );
        REQUIRE( parser.data->lines_ignored == 5 );
    }

    SECTION( "Vertex records" )
    {
        std::ifstream vertices( "vertices.obj" );
        auto parser = model_parser::obj( vertices );

        REQUIRE( parser.error == nullptr );
        REQUIRE( parser.data->vertices[0] == f_point( -1, 1, 0 ) );
        REQUIRE( parser.data->vertices[1] == f_point( -1, 0.5, 0 ) );
        REQUIRE( parser.data->vertices[2] == f_point( 1, 0, 0 ) );
        REQUIRE( parser.data->vertices[3] == f_point( 1, 1, 0 ) );
    }

    SECTION( "Parsing triangle faces" )
    {
        std::ifstream vertices( "triangles.obj" );
        auto parser = model_parser::obj( vertices );

        REQUIRE( parser.error == nullptr );

        auto g = parser.data->root_group;
        auto t1 = std::static_pointer_cast<triangle>( g->children()[0] );
        auto t2 = std::static_pointer_cast<triangle>( g->children()[1] );
        
        REQUIRE( t1->p1() == parser.data->vertices[0] );
        REQUIRE( t1->p2() == parser.data->vertices[1] );
        REQUIRE( t1->p3() == parser.data->vertices[2] );
        REQUIRE( t2->p1() == parser.data->vertices[0] );
        REQUIRE( t2->p2() == parser.data->vertices[2] );
        REQUIRE( t2->p3() == parser.data->vertices[3] );
    }

    SECTION( "Triangulating polygons" )
    {
        std::ifstream vertices( "polygon.obj" );
        auto parser = model_parser::obj( vertices );

        REQUIRE( parser.error == nullptr );

        auto g = parser.data->root_group;
        auto t1 = std::static_pointer_cast<triangle>( g->children()[0] );
        auto t2 = std::static_pointer_cast<triangle>( g->children()[1] );
        auto t3 = std::static_pointer_cast<triangle>( g->children()[2] );

        REQUIRE( t1->p1() == parser.data->vertices[0] );
        REQUIRE( t1->p2() == parser.data->vertices[1] );
        REQUIRE( t1->p3() == parser.data->vertices[2] );
        REQUIRE( t2->p1() == parser.data->vertices[0] );
        REQUIRE( t2->p2() == parser.data->vertices[2] );
        REQUIRE( t2->p3() == parser.data->vertices[3] );
        REQUIRE( t3->p1() == parser.data->vertices[0] );
        REQUIRE( t3->p2() == parser.data->vertices[3] );
        REQUIRE( t3->p3() == parser.data->vertices[4] );
    }

    SECTION( "Triangles in groups" )
    {
        std::ifstream vertices( "groups.obj" );
        auto parser = model_parser::obj( vertices );

        REQUIRE( parser.error == nullptr );

        auto g1 = parser.data->groups["FirstGroup"];
        auto g2 = parser.data->groups["SecondGroup"];
        auto t1 = std::static_pointer_cast<triangle>( g1->children()[0] );
        auto t2 = std::static_pointer_cast<triangle>( g2->children()[0] );

        REQUIRE( t1->p1() == parser.data->vertices[0] );
        REQUIRE( t1->p2() == parser.data->vertices[1] );
        REQUIRE( t1->p3() == parser.data->vertices[2] );
        REQUIRE( t2->p1() == parser.data->vertices[0] );
        REQUIRE( t2->p2() == parser.data->vertices[2] );
        REQUIRE( t2->p3() == parser.data->vertices[3] );
    }

    SECTION( "Converting an OBJ file to a group" )
    {
        std::ifstream vertices( "groups.obj" );
        auto parser = model_parser::obj( vertices );

        REQUIRE( parser.error == nullptr );

        auto g = parser.to_shape_group();
        auto it1 = std::find_if( g->children().cbegin(), g->children().cend(), [] ( const shape_ptr s ) {
            auto sub_group = std::dynamic_pointer_cast<group>( s );
            return sub_group && sub_group->name() == "FirstGroup";
        } );
        auto it2 = std::find_if( g->children().cbegin(), g->children().cend(), [] ( const shape_ptr s ) {
            auto sub_group = std::dynamic_pointer_cast<group>( s );
            return sub_group && sub_group->name() == "SecondGroup";
        } );

        REQUIRE( it1 != g->children().cend() );
        REQUIRE( it2 != g->children().cend() );
    }
};