#include <iostream>
#include <chrono>
#include <thread>
#include "tensor.hpp"
#include "transform.hpp"
#include "canvas.hpp"

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

int main( int argc, char* argv[] )
{
	uint16_t canvas_width = 900, canvas_height = 550;
	auto canv = ls::canvas( canvas_width, canvas_height );

	// 1. The projectile sample runs a basic physics projectile launch simulation
	// and draws a small dot every frame showing where the projectile is, outputting
	// the entire trajectory to an image
	//run_projectile_sample( canv );

	// 2. The clock sample draws twelve small dots at the usual analog clock hour locations
	// and demonstrate the use of transforms
	run_clock_sample( canv );

	return 0;
}