#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

struct Request
{
    std::string customerID;
    int tenure;
    double monthlyCharges;
    double totalCharges;
    std::string churn;
};

#endif