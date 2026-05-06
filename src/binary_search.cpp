#include "binary_search.hpp"

static int buscarRecursivo(const std::vector<Solicitud> &solicitudes, int izquierda, int derecha, int k, int respuesta)
{
    if (izquierda > derecha)
    {
        return respuesta;
    }

    int medio = izquierda + (derecha - izquierda) / 2;

    if (solicitudes[medio].antigüedad >= k)
    {
        respuesta = medio;
        return buscarRecursivo(solicitudes, izquierda, medio - 1, k, respuesta);
    }

    return buscarRecursivo(solicitudes, izquierda, medio - 1, k, respuesta);
}

int buscarPorAntigüedad(const std::vector<Solicitud> &solicitudes, int k)
{
    if (solicitudes.empty())
    {
        return -1;
    }

    return buscarRecursivo(solicitudes, 0, solicitudes.size() - 1, k, -1);
}