#pragma once

#include <vector>
#include "common.hpp"
#include "shapes.hpp"
#include "lights.hpp"
#include "transform.hpp"

namespace ls {
    class world
    {
    public:

        using ptr = std::shared_ptr<world>;

    public:

        const light::ptr light() const noexcept
        {
            return _light;
        }

        const std::vector<shape::ptr>& objects() const noexcept
        {
            return _objects;
        }

        bool contains( const shape::ptr& s ) const;

        static ptr create_default() noexcept;

        template<typename... Ts>
        static ptr create( Ts&&... args ) noexcept
        {
            return ptr( new world( std::forward<Ts>( args )... ) );
        }

    private:

        light::ptr _light = nullptr;
        std::vector<shape::ptr> _objects;

    };
}