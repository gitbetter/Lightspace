#pragma once

#include "common.hpp"
#include "tensor.hpp"
#include "patterns.hpp"

namespace ls {
    class material 
    {
    public:
        
        pattern_ptr surface_pattern;
        fpnum reflectivity;
        fpnum transparency;
        fpnum refractive_index;
        
    public:
        
        material() :
            surface_pattern( solid_pattern::create() ), reflectivity( 0.f ), transparency( 0.f ), refractive_index( 1.f )
        { }
        material( const pattern_ptr& patt ) :
            surface_pattern( patt ), reflectivity( 0.f ), transparency( 0.f ), refractive_index( 1.f )
        { }
        virtual ~material()
        { }
        
    };

    class phong_material : public material
    {
    public:

        fpnum ambient;
        fpnum diffuse;
        fpnum specular;
        fpnum shininess;

    public:

        phong_material() :
            material(), ambient( 0.1f ), diffuse( 0.9f ), specular( 0.9f ), shininess( 200.f )
        { }
        phong_material( const f_color& col, fpnum amb = 0.1f, fpnum diff = 0.9f, fpnum spec = 0.9f, fpnum shine = 200.f ) :
            material( solid_pattern::create( col ) ), ambient( amb ), diffuse( diff ), specular( spec ), shininess( shine )
        { }
        phong_material( const pattern_ptr& patt, fpnum amb = 0.1f, fpnum diff = 0.9f, fpnum spec = 0.9f, fpnum shine = 200.f ) :
            material( patt ), ambient( amb ), diffuse( diff ), specular( spec ), shininess( shine )
        { }

        bool operator==( const phong_material& rhs ) const noexcept
        {
            return approx( ambient, rhs.ambient ) && approx( diffuse, rhs.diffuse ) &&
                    approx( specular, rhs.specular ) && approx( shininess, rhs.shininess );
        }

        PTR_FACTORY( phong_material )

    };
}
