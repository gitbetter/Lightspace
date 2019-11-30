#pragma once

#include <vector>
#include "common.hpp"
#include "shapes.hpp"
#include "lights.hpp"
#include "transform.hpp"
#include "intersection.hpp"

namespace ls {
    class world : public std::enable_shared_from_this<world>
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

        void add_object( shape_ptr obj );

        void remove_object( shape_ptr obj );

        bool contains( const shape_ptr& s ) const;

        static world_ptr create_default() noexcept;

        f_color shade_hit( const intersection_state& state );

        f_color color_at( const ray& r );

        bool in_shadow( const f_point& p );

        PTR_FACTORY( world )

    private:

        light_ptr _light = nullptr;
        std::vector<shape_ptr> _objects;

    };

    intersections intersect( const world_ptr& s, const ray& r );
}