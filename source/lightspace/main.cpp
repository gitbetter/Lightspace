#include <iostream>
#include <chrono>
#include <thread>
#include "tensor.hpp"
#include "transform.hpp"
#include "canvas.hpp"
#include "shapes.hpp"
#include "ray.hpp"
#include "intersection.hpp"

using namespace std;

struct projectile
{
	ls::f_point position;
	ls::f_vector velocity;

	projectile( ls::f_point position, ls::f_vector velocity ) :
		position( position ), velocity( velocity )
	{ }
};

struct environment
{
	ls::f_vector gravity;
	ls::f_vector wind;

	environment( ls::f_vector gravity, ls::f_vector wind ) :
		gravity( gravity ), wind( wind )
	{ }
};

projectile tick( const projectile& proj, const environment& env )
{
	ls::f_point position = proj.position + proj.velocity;
	ls::f_vector velocity = proj.velocity + env.gravity + env.wind;
	return projectile( position, velocity );
}

void run_projectile_sample( ls::canvas& canv )
{
	using namespace this_thread;
	using namespace chrono_literals;

	auto start_point = ls::f_point( 0, 1, 0 );
	auto start_velocity = ls::f_vector( 1, 1.8f, 0 ).normalized() * 11.25f;
	auto proj = projectile( start_point, start_velocity );

	auto gravity = ls::f_vector( 0, -0.1f, 0 );
	auto wind = ls::f_vector( -0.01f, 0, 0 );
	auto env = environment( gravity, wind );

	auto pixel_color = ls::f_color( 1, 0, 0 );

	unsigned int ticks = 0;
	while ( true )
	{
		proj = tick( proj, env );
		++ticks;

		cout << "[Projectile Position]: " << proj.position << endl;

		canv.draw_pixel( static_cast<uint16_t>( proj.position.x ), static_cast<uint16_t>( canv.height() - proj.position.y ), pixel_color );

		if ( proj.position.y <= 0 )
		{
			break;
		}

		sleep_for( 50ms );
	}

	canv.write_to( "projectile_render.ppm" );
}

void run_clock_sample( ls::canvas& canv )
{
	auto pixel_color = ls::f_color( 1, 1, 1 );
	auto start_point = ls::f_point( 0, 1, 0 );
	fpnum radius = 400.f;

	for ( uint8_t i = 0; i < 12; i++ )
	{
		auto rot = ls::transform::rotation_z( ls::pi_over_6 * i );
		auto new_point = rot * start_point;
		new_point = (new_point * radius + ls::f_vector( canv.width(), canv.height(), 0 )) / 2;
		canv.draw_pixel( static_cast<uint16_t>( new_point.x ), static_cast<uint16_t>( new_point.y ), pixel_color );
	}

	canv.write_to( "clock_render.ppm" );
}

void run_simple_sphere_sample( ls::canvas& canv )
{
    auto pixel_color = ls::f_color( 1, 0, 0 );
    auto sph = ls::sphere::create();
    auto wall_size = 7.f;
    auto pixel_size = wall_size / canv.width();
    auto half = wall_size * 0.5f;
    auto ray_origin = ls::f_point( 0, 0, -5 );

    for ( auto i = 0; i < canv.width(); i++ )
    {
        auto world_x = -half + pixel_size * i;
        for ( auto j = 0; j < canv.height(); j++ )
        {
            auto world_y = half - pixel_size * j;
            auto wall_position = ls::f_point( world_x, world_y, 10.f );
            auto ray = ls::ray( ray_origin, ( wall_position - ray_origin ).normalized() );
            auto hit = ls::hit( ls::intersect( sph, ray ) );
            if ( hit != ls::intersection::none )
            {
                canv.draw_pixel( i, j, pixel_color );
            }
        }
    }

    canv.write_to( "simple_sphere_render.ppm" );
}


int main( int argc, char* argv[] )
{
	uint16_t canvas_width = 300, canvas_height = 300;
	auto canv = ls::canvas( canvas_width, canvas_height );

	// 1. The projectile sample runs a basic physics projectile launch simulation
	// and draws a small dot every frame showing where the projectile is, outputting
	// the entire trajectory to an image
	//run_projectile_sample( canv );

	// 2. The clock sample draws twelve small dots at the usual analog clock hour locations
	// and demonstrate the use of transforms
	//run_clock_sample( canv );

    // 3. The simple sphere sample draws the sillouette of a sphere onto the canvas, without any
    // special lighting or shading.
    run_simple_sphere_sample( canv );

	return 0;
}