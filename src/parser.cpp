#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

static std::vector<std::string> splitCSVLine(const std::string &line)
{
    std::vector<std::string> fields;
    std::string field;
    bool insideQuotes = false;

    for (char c : line)
    {
        if (c == '"')
        {
            insideQuotes = !insideQuotes;
        }
        else if (c == ',' && !insideQuotes)
        {
            fields.push_back(field);
            field.clear();
        }
        else
        {
            field += c;
        }
    }

    fields.push_back(field);
    return fields;
}

static double parseTotalCharges(const std::string &value, int &nullCounter)
{
    if (value.empty() || value == " ")
    {
        nullCounter++;
        return 0.0;
    }

    try
    {
        return std::stod(value);
    }
    catch (...)
    {
        nullCounter++;
        return 0.0;
    }
}

ParseResult readCSV(const std::string &path)
{
    std::ifstream file(path);

    ParseResult result;
    result.nullTotalCharges = 0;

    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir el archivo: " << path << std::endl;
        return result;
    }

    std::string line;

    getline(file, line);

    while (getline(file, line))
    {
        std::vector<std::string> fields = splitCSVLine(line);

        if (fields.size() < 21)
        {
            continue;
        }

        Request request;

        request.customerID = fields[0];
        request.tenure = std::stoi(fields[5]);
        request.monthlyCharges = std::stod(fields[18]);
        request.totalCharges = parseTotalCharges(fields[19], result.nullTotalCharges);
        request.churn = fields[20];

        result.requests.push_back(request);
    }

    file.close();

    return result;
}