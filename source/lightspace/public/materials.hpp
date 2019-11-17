#pragma once

#include "common.hpp"
#include "tensor.hpp"

namespace ls {
    class material 
    { };

    class phong_material : public material
    {
    public:

        f_color surface_color;
        fpnum ambient;
        fpnum diffuse;
        fpnum specular;
        fpnum shininess;

    public:

        phong_material() :
            surface_color( f_color( 1, 1, 1 ) ), ambient( 0.1f ), diffuse( 0.9f ), specular( 0.9f ), shininess( 200.f )
        { }
        phong_material( const f_color& col, fpnum amb = 0.1f, fpnum diff = 0.9f, fpnum spec = 0.9f, fpnum shine = 200.f ) :
            surface_color( col ), ambient( amb ), diffuse( diff ), specular( spec ), shininess( shine )
        { }

        bool operator==( const phong_material& rhs ) const noexcept
        {
            return surface_color == rhs.surface_color && approx( ambient, rhs.ambient ) && approx( diffuse, rhs.diffuse ) &&
                approx( specular, rhs.specular ) && approx( shininess, rhs.shininess );
        }

        PTR_FACTORY( phong_material )

    };
}