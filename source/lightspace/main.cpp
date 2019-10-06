#include <iostream>
#include <chrono>
#include <thread>
#include "tensor.hpp"

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

	auto proj = projectile( ls::f_point( 0, 1, 0 ), ls::f_vector( 1, 1, 0 ) );
	auto env = environment( ls::f_vector( 0, -0.1f, 0 ), ls::f_vector( -0.01f, 0, 0 ) );

	unsigned int ticks = 0;
	while ( true )
	{
		proj = tick( proj, env );
		++ticks;

		cout << "[Projectile Position]: " << proj.position << endl;

		if ( proj.position.y <= 0 )
		{
			cout << "Projectile hit the ground in " << ticks << " ticks." << endl;
			break;
		}

		sleep_for( 50ms );
	}

	return 0;
}