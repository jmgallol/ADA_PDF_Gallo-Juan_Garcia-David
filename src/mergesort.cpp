#include "mergesort.hpp"

static void fusionar(std::vector<Solicitud> &arr, int izquierda, int medio, int derecha)
{
    int n1 = medio - izquierda + 1;
    int n2 = derecha - medio;

    std::vector<Solicitud> arrayIzquierdo(n1);
    std::vector<Solicitud> arrayDerecho(n2);

    for (int i = 0; i < n1; i++)
    {
        arrayIzquierdo[i] = arr[izquierda + i];
    }

    for (int j = 0; j < n2; j++)
    {
        arrayDerecho[j] = arr[medio + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = izquierda;

    while (i < n1 && j < n2)
    {
        if (arrayIzquierdo[i].antigüedad <= arrayDerecho[j].antigüedad)
        {
            arr[k] = arrayIzquierdo[i];
            i++;
        }
        else
        {
            arr[k] = arrayDerecho[j];
            j++;
        }

        k++;
    }

    while (i < n1)
    {
        arr[k] = arrayIzquierdo[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = arrayDerecho[j];
        j++;
        k++;
    }
}

static void fusionacionRecursiva(std::vector<Solicitud> &arr, int izquierda, int derecha)
{
    if (izquierda >= derecha)
    {
        return;
    }

    int medio = izquierda + (derecha - izquierda) / 2;

    fusionacionRecursiva(arr, izquierda, medio);
    fusionacionRecursiva(arr, medio + 1, derecha);

    fusionar(arr, izquierda, medio, derecha);
}

void fusionacionPorAntigüedadDesc(std::vector<Solicitud> &solicitudes)
{
    if (solicitudes.empty())
    {
        return;
    }

    fusionacionRecursiva(solicitudes, 0, solicitudes.size() - 1);
}