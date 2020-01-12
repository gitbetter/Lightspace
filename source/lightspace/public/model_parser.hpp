#pragma once

#include "common.hpp"
#include "tensor.hpp"
#include <fstream>
#include <vector>
#include <map>

namespace ls {
    enum class model_parse_status
    {
        SUCCESS, FAIL
    };

    struct model_parse_data
    {
        unsigned int lines_ignored;
        std::vector<f_point> vertices;
        std::map<std::string, group_ptr> groups;
        group_ptr root_group;

        model_parse_data();
    };

    struct model_parse_error
    {
        unsigned int line_number{ 0 };
        std::string message;
    };


    struct model_parse_result
    {
        model_parse_status status{ model_parse_status::FAIL };
        std::unique_ptr<model_parse_data> data;
        std::unique_ptr<model_parse_error> error;

        group_ptr to_shape_group() const;
    };

    struct model_parser
    {
        static model_parse_result obj( std::ifstream& f );
    };
}
