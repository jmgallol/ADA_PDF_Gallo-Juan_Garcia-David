#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include "request.hpp"

struct ResultadoAnalisis
{
    std::vector<Solicitud> solicitudes;
    int cargosTotalesNulos;
};

ResultadoAnalisis leerCSV(const std::string &ruta);

#endif