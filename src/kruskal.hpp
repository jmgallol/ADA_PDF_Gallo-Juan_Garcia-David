#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <vector>
#include "request.hpp"

struct Arista
{
    int u, v;
    int costo;
    bool operator<(const Arista &otro) const
    {
        if (costo != otro.costo)
            return costo < otro.costo;
        if (u != otro.u)
            return u < otro.u;
        return v < otro.v;
    }
};

void ejecutarModuloB(const std::vector<Solicitud> &solicitudes);

#endif
