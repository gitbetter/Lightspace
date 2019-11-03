#pragma once

#include "common.hpp"
#include "tensor.hpp"

namespace ls {
    class material 
    { };

    class phong_material : public material
    {
    public:

        using ptr = std::shared_ptr<phong_material>;

    public:

        f_color color;
        fpnum ambient;
        fpnum diffuse;
        fpnum specular;
        fpnum shininess;

    public:

        phong_material() :
            color( f_color( 1, 1, 1 ) ), ambient( 0.1f ), diffuse( 0.9f ), specular( 0.9f ), shininess( 200.f )
        { }

        bool operator==( const phong_material& rhs ) const noexcept
        {
            return color == rhs.color && approx( ambient, rhs.ambient ) && approx( diffuse, rhs.diffuse ) &&
                approx( specular, rhs.specular ) && approx( shininess, rhs.shininess );
        }

    };
}