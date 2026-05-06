#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "parser.hpp"
#include "mergesort.hpp"
#include "binary_search.hpp"
#include "kruskal.hpp"
#include "knapsack.hpp"

using namespace std;

void crearArchivoOrdenado(const vector<Solicitud> &solicitudes)
{
    ofstream archivo("results/solicitudes_ordenadas.csv");

    if (!archivo.is_open())
    {
        cout << "Error: no se pudo crear results/solicitudes_ordenadas.csv" << endl;
        return;
    }

    archivo << "idCliente,antigedad,cargosMenuales,cargosTotales,rotacion\n";

    for (const Solicitud &s : solicitudes)
    {
        archivo << s.idCliente << ","
                << s.antigüedad << ","
                << s.cargosMenuales << ","
                << s.cargosTotales << ","
                << s.rotacion << "\n";
    }

    archivo.close();

    cout << "Archivo creado: results/solicitudes_ordenadas.csv" << endl;
}

void crearArchivoBusquedas(const vector<Solicitud> &solicitudes)
{
    ofstream archivo("results/busquedas_A.txt");

    if (!archivo.is_open())
    {
        cout << "Error: no se pudo crear results/busquedas_A.txt" << endl;
        return;
    }

    int consultas[5] = {72, 60, 45, 30, 12};
    string nombres[5] = {"Q_A01", "Q_A02", "Q_A03", "Q_A04", "Q_A05"};

    for (int i = 0; i < 5; i++)
    {
        int posicion = buscarPorAntigüedad(solicitudes, consultas[i]);

        archivo << nombres[i] << " k=" << consultas[i] << " ";

        if (posicion != -1)
        {
            archivo << "idCliente=" << solicitudes[posicion].idCliente
                    << " antigedad=" << solicitudes[posicion].antigüedad << "\n";
        }
        else
        {
            archivo << "No encontrado\n";
        }
    }

    archivo.close();

    cout << "Archivo creado: results/busquedas_A.txt" << endl;
}

int main(int argc, char *argv[])
{
    cout << "Iniciando Modulo A..." << endl;

    if (argc < 2)
    {
        cout << "Error: debes pasar la ruta del CSV" << endl;
        cout << "Ejemplo:" << endl;
        cout << "./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv" << endl;
        return 1;
    }

    string rutaCSV = argv[1];

    cout << "Leyendo archivo: " << rutaCSV << endl;

    ResultadoAnalisis resultado = leerCSV(rutaCSV);

    cout << "Registros cargados: " << resultado.solicitudes.size() << endl;
    cout << "CargosTotales nulos: " << resultado.cargosTotalesNulos << endl;

    if (resultado.solicitudes.empty())
    {
        cout << "Error: no se cargo ningun registro. Revisa la ruta o el parser." << endl;
        return 1;
    }

    cout << "Ordenando solicitudes con MergeSort..." << endl;

    fusionacionPorAntigüedadDesc(resultado.solicitudes);

    cout << "Primer registro ordenado:" << endl;
    cout << resultado.solicitudes[0].idCliente << " antigedad=" << resultado.solicitudes[0].antigüedad << endl;

    crearArchivoOrdenado(resultado.solicitudes);
    crearArchivoBusquedas(resultado.solicitudes);

    cout << "Ejecutando Modulo B..." << endl;
    ejecutarModuloB(resultado.solicitudes);

    cout << "Ejecutando Modulo C..." << endl;
    ejecutarModuloC(resultado.solicitudes);

    cout << "Todos los modulos finalizados." << endl;

    return 0;
}