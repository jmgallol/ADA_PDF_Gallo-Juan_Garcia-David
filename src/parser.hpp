#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include "request.hpp"

struct ParseResult
{
    std::vector<Request> requests;
    int nullTotalCharges;
};

ParseResult readCSV(const std::string &path);

#endif