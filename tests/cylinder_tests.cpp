#include "catch.hpp"
#include "shapes.hpp"

using namespace ls;

TEST_CASE( "Cylinder processing", "[cylinders]" )
{
    SECTION( "A ray misses a cylinder" )
    {
        auto cyl = cylinder::create();
        auto r = ray( f_point( 0, 0, -5 ), f_vector( 1, 1, 1 ) );
        auto itrs = intersect( cyl, r );
        
        REQUIRE( itrs.size() == 0 );
    }
    
    SECTION( "A ray strikes a cylinder" )
    {
        auto cyl = cylinder::create();
        auto r = ray( f_point( 1, 0, -5 ), f_vector( 0, 0, 1 ) );
        auto itrs = intersect( cyl, r );
        
        REQUIRE( itrs.size() == 2 );
        REQUIRE( itrs[0].time() == 5 );
        REQUIRE( itrs[1].time() == 5 );
    }
    
    SECTION( "Normal vector on a cylinder" )
    {
        auto cyl = cylinder::create();
        auto n = cyl->normal( -1, 1, 0 );

        REQUIRE( n == f_vector( -1, 0, 0 ) );
    }
    
    SECTION( "The default minimum and maximum for a cylinder" )
    {
        auto cyl = cylinder::create();
        
        REQUIRE( cyl->min_extent() == -infinity );
        REQUIRE( cyl->max_extent() == infinity );
    }
    
    SECTION( "Intersecting a constrained cylinder" )
    {
        auto cyl = cylinder::create();
        cyl->set_min_extent( 1 );
        cyl->set_max_extent( 2 );
        auto r = ray( f_point( 0, 1.5f, -2 ), f_vector( 0, 0, 1 ).normalized() );
        auto itrs = intersect( cyl, r );
        
        REQUIRE( itrs.size() == 2 );
    }
    
    SECTION( "The default closed value for a cylinder" )
    {
        auto cyl = cylinder::create();
        
        REQUIRE( !cyl->closed() );
    }
    
    SECTION( "Intersecting the caps of a closed cylinder" )
    {
        auto cyl = cylinder::create();
        cyl->set_min_extent( 1 );
        cyl->set_max_extent( 2 );
        cyl->set_closed( true );
        auto r = ray( f_point( 0, 4, -2 ), f_vector( 0, -1, 1 ).normalized() );
        auto itrs = intersect( cyl, r );
        
        REQUIRE( itrs.size() == 2 );
    }
    
    SECTION( "The normal vector on a cylinder's end caps" )
    {
        auto cyl = cylinder::create();
        cyl->set_min_extent( 1 );
        cyl->set_max_extent( 2 );
        cyl->set_closed( true );
        auto n = cyl->normal( 0, 2, 0.5f );
        
        REQUIRE( n == f_vector( 0, 1, 0 ) );
    }
};
