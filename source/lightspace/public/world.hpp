#pragma once

#include <vector>
#include "common.hpp"
#include "shapes.hpp"
#include "lights.hpp"
#include "transform.hpp"
#include "intersection.hpp"

namespace ls {
    class world
    {
    public:

        const light_ptr light() const noexcept
        {
            return _light;
        }

        void set_light( light_ptr light ) noexcept
        {
            _light = light;
        }

        const std::vector<shape_ptr>& objects() const noexcept
        {
            return _objects;
        }

        bool contains( const shape_ptr& s ) const;

        static world_ptr create_default() noexcept;

        f_color shade_hit( const intersection_state& state );

        PTR_FACTORY( world )

    private:

        light_ptr _light = nullptr;
        std::vector<shape_ptr> _objects;

    };

    intersections intersect( const world_ptr& s, const ray& r );
}