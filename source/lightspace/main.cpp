#include <iostream>
#include <chrono>
#include <thread>
#include "tensor.hpp"
#include "transform.hpp"
#include "canvas.hpp"
#include "shapes.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "materials.hpp"
#include "lights.hpp"
#include "world.hpp"
#include "camera.hpp"

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

void run_projectile_sample( uint16_t x_res, uint16_t y_res )
{
    using namespace this_thread;
    using namespace chrono_literals;

    auto canv = ls::canvas( x_res, y_res );
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

void run_clock_sample( uint16_t x_res, uint16_t y_res )
{
    auto canv = ls::canvas( x_res, y_res );
    auto pixel_color = ls::f_color( 1, 1, 1 );
    auto start_point = ls::f_point( 0, 1, 0 );
    fpnum radius = 400.f;

    for ( uint8_t i = 0; i < 12; i++ )
    {
        auto rot = ls::transform::rotation_z( ls::pi_over_6 * i );
        auto new_point = rot * start_point;
        new_point = ( new_point * radius + ls::f_vector( canv.width(), canv.height(), 0 ) ) / 2;
        canv.draw_pixel( static_cast<uint16_t>( new_point.x ), static_cast<uint16_t>( new_point.y ), pixel_color );
    }

    canv.write_to( "clock_render.ppm" );
}

void run_simple_sphere_sample( uint16_t x_res, uint16_t y_res )
{
    auto canv = ls::canvas( x_res, y_res );
    auto sph = ls::sphere::create();
    auto wall_size = 7.f;
    auto pixel_size = wall_size / canv.width();
    auto half = wall_size * 0.5f;
    auto ray_origin = ls::f_point( 0, 0, -5 );
    auto scene_light = ls::point_light::create( ls::f_color( 1, 1, 1 ), ls::f_point( 10, 10, -10 ) );

    auto mat = ls::phong_material();
    mat.surface_color = ls::f_color( 1.f, 0.65f, 0.3f );
    mat.shininess = 125.f;
    sph->set_material( mat );

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
                auto pos = ray.position( hit.time() );
                auto norm = hit.object()->normal( pos.x, pos.y, pos.z );
                auto eye = -ray.direction();
                auto pixel_color = ls::phong_lighting( hit.object()->material(), scene_light, pos, eye, norm );
                canv.draw_pixel( i, j, pixel_color );
            }
        }
    }

    canv.write_to( "simple_sphere_render.ppm" );
}

void run_simple_scene_sample( uint16_t x_res, uint16_t y_res )
{
    auto floor = ls::sphere::create();
    floor->set_transform( ls::transform::scale( 10.f, 0.01f, 10.f ) );
    auto floor_mat = ls::phong_material();
    floor_mat.surface_color = ls::f_color( 1.f, 0.9f, 0.9f );
    floor_mat.specular = 0.f;
    floor->set_material( floor_mat );

    auto left_wall = ls::sphere::create();
    left_wall->set_transform( ls::transform::translation( 0.f, 0.f, 0.5f ) * 
                              ls::transform::rotation_y( -ls::pi_over_4 ) * ls::transform::rotation_x( ls::pi_over_2 ) * 
                              ls::transform::scale( 10.f, 0.01f, 10.f ) );
    left_wall->set_material( floor_mat );

    auto right_wall = ls::sphere::create();
    right_wall->set_transform( ls::transform::translation( 0.f, 0.f, 0.5f ) * 
                               ls::transform::rotation_y( ls::pi_over_4 ) * ls::transform::rotation_x( ls::pi_over_2 ) * 
                               ls::transform::scale( 10.f, 0.01f, 10.f ) );
    right_wall->set_material( floor_mat );

    auto middle_sphere = ls::sphere::create();
    middle_sphere->set_transform( ls::transform::translation( -0.5f, 1.f, 0.5f ) );
    auto middle_sphere_mat = ls::phong_material();
    middle_sphere_mat.surface_color = ls::f_color( 0.1f, 1.f, 0.5f );
    middle_sphere_mat.diffuse = 0.7f;
    middle_sphere_mat.specular = 0.3f;
    middle_sphere->set_material( middle_sphere_mat );

    auto right_sphere = ls::sphere::create();
    right_sphere->set_transform( ls::transform::translation( 1.5f, 0.5f, -0.5f ) * 
                                 ls::transform::scale( 0.5f, 0.5f, 0.5f ) );
    auto right_sphere_mat = ls::phong_material();
    right_sphere_mat.surface_color = ls::f_color( 0.5f, 1.f, 0.1f );
    right_sphere_mat.diffuse = 0.7f;
    right_sphere_mat.specular = 0.3f;
    right_sphere->set_material( right_sphere_mat );

    auto left_sphere = ls::sphere::create();
    left_sphere->set_transform( ls::transform::translation( -1.5f, 0.33f, -0.75f ) *
                                 ls::transform::scale( 0.33f, 0.33f, 0.33f ) );
    auto left_sphere_mat = ls::phong_material();
    left_sphere_mat.surface_color = ls::f_color( 1.f, 0.8f, 0.1f );
    left_sphere_mat.diffuse = 0.7f;
    left_sphere_mat.specular = 0.3f;
    left_sphere->set_material( left_sphere_mat );

    auto light = ls::point_light::create( ls::f_color( 1.f, 1.f, 1.f ), ls::f_point( -10.f, 10.f, -10.f ) );

    auto w = ls::world::create();

    w->add_object( floor );
    w->add_object( left_wall );
    w->add_object( right_wall );
    w->add_object( left_sphere );
    w->add_object( middle_sphere );
    w->add_object( right_sphere );
    w->set_light( light );

    auto cam = ls::camera::create( x_res, y_res, ls::pi_over_3 );
    cam->set_transform( ls::transform::view( ls::f_point( 0.f, 1.5f, -5.f ), ls::f_point( 0.f, 1.f, 0.f ), ls::f_vector( 0.f, 1.f, 0.f ) ) );

    auto canv = cam->render( w );
    
    canv.write_to( "simple_scene_render.ppm" );
}


int main( int argc, char* argv[] )
{
    uint16_t canvas_width = 300, canvas_height = 300;

    // 1. The projectile sample runs a basic physics projectile launch simulation
    // and draws a small dot every frame showing where the projectile is, outputting
    // the entire trajectory to an image
    //run_projectile_sample( canvas_width, canvas_height );

    // 2. The clock sample draws twelve small dots at the usual analog clock hour locations
    // and demonstrate the use of transforms
    //run_clock_sample( canvas_width, canvas_height );

    // 3. The simple sphere sample draws a simple sphere onto the canvas, using the phong lighting
    // model to color the sphere.
    // run_simple_sphere_sample( canvas_width, canvas_height );

    // 4. Draws a scene by adding additional walls and spheres to the world.
    run_simple_scene_sample( canvas_width, canvas_height );

    return 0;
}