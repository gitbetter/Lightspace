#include <iostream>
#include <chrono>
#include <thread>
#include "tensor.hpp"
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

int main( int argc, char* argv[] )
{
	using namespace this_thread;
	using namespace chrono_literals;

	uint16_t canvas_width = 900, canvas_height = 550;
	auto canv = ls::canvas( canvas_width, canvas_height );

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

		canv.draw_pixel( static_cast<uint16_t>( proj.position.x ), static_cast<uint16_t>( canvas_height - proj.position.y ), pixel_color );

		if ( proj.position.y <= 0 )
		{
			break;
		}

		sleep_for( 50ms );
	}

	canv.write_to( "projectile_render.ppm" );

	return 0;
}