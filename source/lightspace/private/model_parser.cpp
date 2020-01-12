#include "model_parser.hpp"
#include "shapes.hpp"
#include <sstream>
#include <iterator>

namespace ls {
    model_parse_data::model_parse_data()
    {
        lines_ignored = 0;
        root_group = group::create();
    }

    group_ptr model_parse_result::to_shape_group() const
    {
        auto group = group::create( "Root" );
        if ( !data->root_group->children().empty() )
        {
            group->add_child( data->root_group );
        }
        for ( auto it = data->groups.cbegin(); it != data->groups.cend(); it++ )
        {
            group->add_child( it->second );
        }
        return group;
    }

    model_parse_result model_parser::obj( std::ifstream& f )
    {
        unsigned int line_num = 1;
        model_parse_result result;
        result.data.reset( new model_parse_data );
        group_ptr current_group = result.data->root_group;

        std::string line;
        while ( std::getline( f, line ) )
        {
            std::istringstream iss( line );
            std::vector<std::string> comps( ( std::istream_iterator<std::string>{iss} ),
                                              std::istream_iterator<std::string>() );

            if ( comps.size() == 2 && ( comps[0] == "g" || comps[0] == "G" ) )
            {
                result.data->groups[comps[1]] = group::create( comps[1] );
                current_group = result.data->groups[comps[1]];
            }
            else if ( comps.size() == 4 && ( comps[0] == "v" || comps[0] == "V" ) )
            {
                try
                {
                    fpnum x = std::stof( comps[1] );
                    fpnum y = std::stof( comps[2] );
                    fpnum z = std::stof( comps[3] );
                    result.data->vertices.push_back( f_point( x, y, z ) );
                }
                catch ( const std::invalid_argument& ex )
                {
                    result.data.reset( nullptr );
                    result.error.reset( new model_parse_error );
                    result.error->line_number = line_num;
                    result.error->message = "Invalid argument encountered";
                    return result;
                }
            }
            else if ( comps.size() == 4 && ( comps[0] == "f" || comps[0] == "F" ) )
            {
                try
                {
                    auto p1_idx = std::stoi( comps[1] );
                    auto p2_idx = std::stoi( comps[2] );
                    auto p3_idx = std::stoi( comps[3] );
                    auto t = triangle::create( result.data->vertices[p1_idx - 1], result.data->vertices[p2_idx - 1], result.data->vertices[p3_idx - 1] );
                    current_group->add_child( t );
                }
                catch ( const std::invalid_argument& ex )
                {
                    result.data.reset( nullptr );
                    result.error.reset( new model_parse_error );
                    result.error->line_number = line_num;
                    result.error->message = "Invalid argument encountered";
                    return result;
                }
            }
            else if ( comps.size() > 4 && ( comps[0] == "f" || comps[0] == "F" ) )
            {
                std::vector<f_point> vertices;
                for ( auto i = 1; i < comps.size(); i++ )
                {
                    auto idx = std::stoi( comps[i] );
                    vertices.push_back( result.data->vertices[idx - 1] );
                }

                try
                {
                    for ( auto i = 1; i < vertices.size() - 1; i++ )
                    {
                        auto t = triangle::create( vertices[0], vertices[i], vertices[i + 1] );
                        current_group->add_child( t );
                    }
                }
                catch ( const std::invalid_argument& ex )
                {
                    result.data.reset( nullptr );
                    result.error.reset( new model_parse_error );
                    result.error->line_number = line_num;
                    result.error->message = "Invalid argument encountered";
                    return result;
                }
            }
            else
            {
                result.data->lines_ignored += 1;
            }

            ++line_num;
        }
        return result;
    }
}
