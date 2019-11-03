#include "lights.hpp"

namespace ls {
    f_color phong_lighting( const phong_material& mat, const light& l, const f_point& position, const f_vector& eye, const f_vector& normal )
    {
        auto effective_color = mat.surface_color * l.intensity();
        auto light_v = ( l.position() - position ).normalized();

        auto ambient = effective_color * mat.ambient;
        auto diffuse = f_color( 0, 0, 0 );
        auto specular = f_color( 0, 0, 0 );

        auto light_dot_normal = light_v.dot( normal );
        if ( light_dot_normal >= 0 )
        {
            diffuse = effective_color * mat.diffuse * light_dot_normal;

            auto reflection_v = ( -light_v ).reflect( normal );
            auto reflect_dot_eye = reflection_v.dot( eye );
            if ( reflect_dot_eye > 0 )
            {
                auto factor = powf( reflect_dot_eye, mat.shininess );
                specular = l.intensity() * mat.specular * factor;
            }
        }

        return ambient + diffuse + specular;
    }
}