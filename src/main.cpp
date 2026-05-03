#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "parser.hpp"
#include "mergesort.hpp"
#include "binary_search.hpp"

using namespace std;

void crearArchivoOrdenado(const vector<Request> &requests)
{
    ofstream file("results/solicitudes_ordenadas.csv");

    if (!file.is_open())
    {
        cout << "Error: no se pudo crear results/solicitudes_ordenadas.csv" << endl;
        return;
    }

    file << "customerID,tenure,MonthlyCharges,TotalCharges,Churn\n";

    for (const Request &r : requests)
    {
        file << r.customerID << ","
             << r.tenure << ","
             << r.monthlyCharges << ","
             << r.totalCharges << ","
             << r.churn << "\n";
    }

    file.close();

    cout << "Archivo creado: results/solicitudes_ordenadas.csv" << endl;
}

void crearArchivoBusquedas(const vector<Request> &requests)
{
    ofstream file("results/busquedas_A.txt");

    if (!file.is_open())
    {
        cout << "Error: no se pudo crear results/busquedas_A.txt" << endl;
        return;
    }

    int consultas[5] = {72, 60, 45, 30, 12};
    string nombres[5] = {"Q_A01", "Q_A02", "Q_A03", "Q_A04", "Q_A05"};

    for (int i = 0; i < 5; i++)
    {
        int posicion = recursiveBinarySearchFirstTenureGreaterEqual(requests, consultas[i]);

        file << nombres[i] << " k=" << consultas[i] << " ";

        if (posicion != -1)
        {
            file << "customerID=" << requests[posicion].customerID
                 << " tenure=" << requests[posicion].tenure << "\n";
        }
        else
        {
            file << "No encontrado\n";
        }
    }

    file.close();

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

    string csvPath = argv[1];

    cout << "Leyendo archivo: " << csvPath << endl;

    ParseResult result = readCSV(csvPath);

    cout << "Registros cargados: " << result.requests.size() << endl;
    cout << "TotalCharges nulos: " << result.nullTotalCharges << endl;

    if (result.requests.empty())
    {
        cout << "Error: no se cargo ningun registro. Revisa la ruta o el parser." << endl;
        return 1;
    }

    cout << "Ordenando solicitudes con MergeSort..." << endl;

    mergeSortByTenureDesc(result.requests);

    cout << "Primer registro ordenado:" << endl;
    cout << result.requests[0].customerID << " tenure=" << result.requests[0].tenure << endl;

    crearArchivoOrdenado(result.requests);
    crearArchivoBusquedas(result.requests);

    cout << "Modulo A finalizado." << endl;

    return 0;
}