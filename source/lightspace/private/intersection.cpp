#include "intersection.hpp"
#include "shapes.hpp"
#include <algorithm>

namespace ls {
    const intersection intersection::none = intersection();

    intersection_state prepare_intersection_state( const intersection& i, const ray& r, const intersections& itrs )
    {
        intersection_state state;
        state.time = i.time();
        state.object = i.object();
        state.point = r.position( state.time );
        state.eye = -r.direction();
        state.normal = state.object->normal( state.point.x, state.point.y, state.point.z );

        if ( state.normal.dot( state.eye ) < 0 )
        {
            state.inside = true;
            state.normal = -state.normal;
        }
        else
        {
            state.inside = false;
        }

        state.shifted_point = state.point + ( state.normal * epsilon );
        state.shifted_under_point = state.point - ( state.normal * epsilon );
        state.reflection = r.direction().reflect( state.normal );
        
        std::vector<shape_ptr> shapes;
        intersection h = hit( itrs );
        for ( const intersection& i : itrs )
        {
            if ( i == h )
            {
                state.ridx_from = shapes.empty() ? 1.f : shapes.back()->material()->refractive_index;
            }
            
            auto it = std::find( shapes.begin(), shapes.end(), i.object() );
            if ( it != shapes.end() )
            {
                shapes.erase( it );
            }
            else
            {
                shapes.push_back( i.object() );
            }
            
            if ( i == h )
            {
                state.ridx_to = shapes.empty() ? 1.f : shapes.back()->material()->refractive_index;
                break;
            }
        }

        return state;
    }

    intersection hit( const intersections& itrs )
    {
        auto itrs_sorted = itrs;
        std::sort( itrs_sorted.begin(), itrs_sorted.end(), [] ( intersection i1, intersection i2 ) {
            return i1.time() < i2.time();
        } );
        auto it = itrs_sorted.cbegin();
        while ( it != itrs_sorted.cend() && it->time() < 0 ) ++it;
        return it == itrs_sorted.cend() ? intersection::none : *it;
    }
}
