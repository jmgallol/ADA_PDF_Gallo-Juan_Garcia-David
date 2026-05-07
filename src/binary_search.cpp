#include "binary_search.hpp"

static int buscarRecursivo(const std::vector<Solicitud> &solicitudes, int izquierda, int derecha, int k)
{
    if (izquierda > derecha)
    {
        return -1; // No encontrado
    }

    int medio = izquierda + (derecha - izquierda) / 2;

    if (solicitudes[medio].antigüedad == k)
    {
        return medio; // Encontrado exactamente
    }
    else if (solicitudes[medio].antigüedad > k)
    {
        return buscarRecursivo(solicitudes, izquierda, medio - 1, k);
    }
    else
    {
        return buscarRecursivo(solicitudes, medio + 1, derecha, k);
    }
}

int buscarPorAntigüedad(const std::vector<Solicitud> &solicitudes, int k)
{
    if (solicitudes.empty())
    {
        return -1;
    }

    return buscarRecursivo(solicitudes, 0, solicitudes.size() - 1, k);
}