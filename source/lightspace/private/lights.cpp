#include "lights.hpp"

namespace ls {
    f_color phong_lighting( const shape_ptr obj, const phong_material_ptr& mat, const light_ptr& l, const f_point& position, const f_vector& eye, const f_vector& normal, bool in_shadow )
    {
        f_color color = mat->surface_pattern->color_at( obj, position );
        auto effective_color = color * l->intensity();
        auto light_v = ( l->position() - position ).normalized();

        auto ambient = effective_color * mat->ambient;

        if ( in_shadow )
        {
            return ambient;
        }

        auto diffuse = f_color( 0, 0, 0 );
        auto specular = f_color( 0, 0, 0 );

        auto light_dot_normal = light_v.dot( normal );
        if ( light_dot_normal >= 0 )
        {
            diffuse = effective_color * mat->diffuse * light_dot_normal;

            auto reflection_v = ( -light_v ).reflect( normal );
            auto reflect_dot_eye = reflection_v.dot( eye );
            if ( reflect_dot_eye > 0 )
            {
                auto factor = powf( reflect_dot_eye, mat->shininess );
                specular = l->intensity() * mat->specular * factor;
            }
        }

        return ambient + diffuse + specular;
    }
}
