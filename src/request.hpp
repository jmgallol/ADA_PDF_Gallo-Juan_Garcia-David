#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

struct Solicitud
{
    std::string idCliente;
    int antigüedad;
    double cargosMenuales;
    double cargosTotales;
    std::string rotacion;
};

#endif