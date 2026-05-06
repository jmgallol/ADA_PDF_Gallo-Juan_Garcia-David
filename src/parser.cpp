#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

static std::vector<std::string> dividirLineaCSV(const std::string &linea)
{
    std::vector<std::string> campos;
    std::string campo;
    bool dentroDeComillas = false;

    for (char c : linea)
    {
        if (c == '"')
        {
            dentroDeComillas = !dentroDeComillas;
        }
        else if (c == ',' && !dentroDeComillas)
        {
            campos.push_back(campo);
            campo.clear();
        }
        else
        {
            campo += c;
        }
    }

    campos.push_back(campo);
    return campos;
}

static double analizarCargosTotales(const std::string &valor, int &contadorNulos)
{
    if (valor.empty() || valor == " ")
    {
        contadorNulos++;
        return 0.0;
    }

    try
    {
        return std::stod(valor);
    }
    catch (...)
    {
        contadorNulos++;
        return 0.0;
    }
}

ResultadoAnalisis leerCSV(const std::string &ruta)
{
    std::ifstream archivo(ruta);

    ResultadoAnalisis resultado;
    resultado.cargosTotalesNulos = 0;

    if (!archivo.is_open())
    {
        std::cerr << "No se pudo abrir el archivo: " << ruta << std::endl;
        return resultado;
    }

    std::string linea;

    getline(archivo, linea);

    while (getline(archivo, linea))
    {
        std::vector<std::string> campos = dividirLineaCSV(linea);

        if (campos.size() < 21)
        {
            continue;
        }

        Solicitud solicitud;

        solicitud.idCliente = campos[0];
        solicitud.antigüedad = std::stoi(campos[5]);
        solicitud.cargosMenuales = std::stod(campos[18]);
        solicitud.cargosTotales = analizarCargosTotales(campos[19], resultado.cargosTotalesNulos);
        solicitud.rotacion = campos[20];

        resultado.solicitudes.push_back(solicitud);
    }

    archivo.close();

    return resultado;
}