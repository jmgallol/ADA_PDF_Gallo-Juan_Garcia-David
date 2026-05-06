#include "knapsack.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

// Representa un elemento para el problema de mochila
struct ElementoMochila
{
    int indice; // índice en el arreglo original
    string idCliente;
    int peso;     // TotalCharges redondeado (ancho de banda)
    int valor;    // MonthlyCharges * 10 redondeado (ingreso)
    double razon; // valor/peso (para codicioso)
};

// Extrae las 50 primeras solicitudes activas (Churn = "No")
vector<ElementoMochila> extraerSolicitudesActivas(const vector<Solicitud> &solicitudesOrdenadas)
{
    vector<ElementoMochila> activas;

    for (size_t i = 0; i < solicitudesOrdenadas.size() && activas.size() < 50; i++)
    {
        if (solicitudesOrdenadas[i].rotacion == "No")
        {
            ElementoMochila elem;
            elem.indice = i;
            elem.idCliente = solicitudesOrdenadas[i].idCliente;
            elem.peso = (int)round(solicitudesOrdenadas[i].cargosTotales);
            elem.valor = (int)round(solicitudesOrdenadas[i].cargosMenuales * 10.0);
            elem.razon = (elem.peso > 0) ? (double)elem.valor / elem.peso : 0.0;
            activas.push_back(elem);
        }
    }

    return activas;
}

// Resuelve Mochila 0-1 usando programación dinámica (tabulación)
// Retorna: (valor óptimo, índices de elementos seleccionados, tabla DP)
struct ResultadoMochila
{
    int valorOptimo;
    vector<int> indicesSeleccionados;
    int pesoUsado;
};

ResultadoMochila resolverMochila(const vector<ElementoMochila> &elementos, int C)
{
    int n = elementos.size();
    ResultadoMochila resultado;
    resultado.valorOptimo = 0;
    resultado.pesoUsado = 0;

    // Crear tabla DP: dp[i][w] = valor máximo con i elementos y capacidad w
    vector<vector<int>> dp(n + 1, vector<int>(C + 1, 0));

    // Llenar tabla DP
    for (int i = 1; i <= n; i++)
    {
        for (int w = 0; w <= C; w++)
        {
            // Opción 1: no tomar el elemento i-1
            dp[i][w] = dp[i - 1][w];

            // Opción 2: tomar el elemento i-1 (si cabe)
            if (elementos[i - 1].peso <= w)
            {
                int valorConElemento = dp[i - 1][w - elementos[i - 1].peso] + elementos[i - 1].valor;
                dp[i][w] = max(dp[i][w], valorConElemento);
            }
        }
    }

    resultado.valorOptimo = dp[n][C];

    // Backtracking: recuperar qué elementos fueron seleccionados
    int w = C;
    for (int i = n; i > 0 && w > 0; i--)
    {
        // Si el valor cambió, significa que este elemento fue seleccionado
        if (dp[i][w] != dp[i - 1][w])
        {
            resultado.indicesSeleccionados.push_back(i - 1);
            resultado.pesoUsado += elementos[i - 1].peso;
            w -= elementos[i - 1].peso;
        }
    }

    // Invertir para que quede en orden original
    reverse(resultado.indicesSeleccionados.begin(), resultado.indicesSeleccionados.end());

    return resultado;
}

// Busca un contraejemplo simple: 3 elementos donde codicioso ≠ óptimo
struct Contraejemplo
{
    vector<int> indicesCodicioso; // índices seleccionados por codicioso
    int valorCodicioso;
    vector<int> indicesOptimo; // índices seleccionados por DP
    int valorOptimo;
    bool encontrado; // si se encontró un contraejemplo real
};

Contraejemplo encontrarContraejemplo(const vector<ElementoMochila> &elementos)
{
    Contraejemplo contraejemplo;
    contraejemplo.valorCodicioso = 0;
    contraejemplo.valorOptimo = 0;
    contraejemplo.encontrado = false;

    if (elementos.size() < 3)
        return contraejemplo;

    // Tomar los 3 primeros elementos
    vector<ElementoMochila> tres;
    vector<int> indicesTres;
    for (int i = 0; i < 3; i++)
    {
        tres.push_back(elementos[i]);
        indicesTres.push_back(i);
    }

    // Usar capacidad reducida para el ejemplo
    int C_local = 100;

    // === ENFOQUE CODICIOSO: seleccionar por ratio v/w descendente ===
    vector<int> ordenRazon = {0, 1, 2};
    sort(ordenRazon.begin(), ordenRazon.end(),
         [&tres](int a, int b)
         { return tres[a].razon > tres[b].razon; });

    int pesoCodicioso = 0;
    for (int idx : ordenRazon)
    {
        if (pesoCodicioso + tres[idx].peso <= C_local)
        {
            contraejemplo.indicesCodicioso.push_back(idx);
            contraejemplo.valorCodicioso += tres[idx].valor;
            pesoCodicioso += tres[idx].peso;
        }
    }

    // === ENFOQUE ÓPTIMO: probar todas las 8 combinaciones posibles ===
    int mejorValor = 0;
    int mejorMascara = 0;

    for (int mascara = 0; mascara < 8; mascara++)
    {
        int peso = 0, valor = 0;
        for (int i = 0; i < 3; i++)
        {
            if (mascara & (1 << i))
            {
                peso += tres[i].peso;
                valor += tres[i].valor;
            }
        }
        if (peso <= C_local && valor > mejorValor)
        {
            mejorValor = valor;
            mejorMascara = mascara;
        }
    }

    contraejemplo.valorOptimo = mejorValor;
    for (int i = 0; i < 3; i++)
    {
        if (mejorMascara & (1 << i))
            contraejemplo.indicesOptimo.push_back(i);
    }

    // Verificar si hay contraejemplo real
    contraejemplo.encontrado = (contraejemplo.valorCodicioso < contraejemplo.valorOptimo);

    return contraejemplo;
}

void ejecutarModuloC(const vector<Solicitud> &solicitudesOrdenadas)
{
    auto tiempoInicio = chrono::high_resolution_clock::now();

    const int C = 500; // Capacidad de ancho de banda

    // === ACTIVIDAD 1: Extraer solicitudes y resolver mochila 0-1 ===
    vector<ElementoMochila> elementos = extraerSolicitudesActivas(solicitudesOrdenadas);

    cout << "\n=== Modulo C: Asignacion de Ancho de Banda ===\n";
    cout << "Solicitudes activas encontradas: " << elementos.size() << "\n";

    if (elementos.empty())
    {
        cerr << "Error: no hay solicitudes activas\n";
        return;
    }

    // Resolver mochila 0-1
    ResultadoMochila resultado = resolverMochila(elementos, C);

    cout << "Valor optimo: " << resultado.valorOptimo << "\n";
    cout << "Solicitudes seleccionadas: " << resultado.indicesSeleccionados.size() << "\n";
    cout << "Ancho de banda usado: " << resultado.pesoUsado << " / " << C << "\n";

    // === ACTIVIDAD 2: Encontrar contraejemplo ===
    Contraejemplo contraejemplo = encontrarContraejemplo(elementos);

    auto tiempoFin = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duracion = tiempoFin - tiempoInicio;

    // === GENERAR ARCHIVO DE SALIDA ===
    ofstream archivo("results/asignacion_bw.txt");
    if (!archivo.is_open())
    {
        cerr << "Error: no se pudo crear results/asignacion_bw.txt\n";
        return;
    }

    archivo << "===============================================\n";
    archivo << "MODULO C: ASIGNACION DE ANCHO DE BANDA\n";
    archivo << "===============================================\n\n";

    // === ACTIVIDAD 1: Mochila 0-1 ===
    archivo << "ACTIVIDAD 1: MOCHILA 0-1 POR TABULACION\n";
    archivo << "---------------------------------------\n\n";

    archivo << "Parametros:\n";
    archivo << "  Capacidad (W): " << C << " unidades\n";
    archivo << "  Numero de solicitudes activas (n): " << elementos.size() << "\n";
    archivo << "  Dimensiones tabla DP: (" << (elementos.size() + 1) << ") x (" << (C + 1) << ")\n\n";

    archivo << "Solucion optima:\n";
    archivo << "  Valor optimo total: " << resultado.valorOptimo << " centavos\n";
    archivo << "  Numero de solicitudes: " << resultado.indicesSeleccionados.size() << "\n";
    archivo << "  Ancho de banda utilizado: " << resultado.pesoUsado << " / " << C << "\n\n";

    archivo << "IDs de cliente seleccionados:\n";
    for (int idx : resultado.indicesSeleccionados)
    {
        archivo << "  - " << elementos[idx].idCliente
                << " (peso=" << elementos[idx].peso
                << ", valor=" << elementos[idx].valor
                << ", ratio=" << fixed << setprecision(2) << elementos[idx].razon << ")\n";
    }

    // === ACTIVIDAD 2: Contraejemplo ===
    archivo << "\n\nACTIVIDAD 2: FALLO DEL ENFOQUE CODICIOSO\n";
    archivo << "-----------------------------------------\n\n";

    archivo << "Contraejemplo con 3 solicitudes:\n\n";
    archivo << "Elementos considerados:\n";
    for (int i = 0; i < 3 && i < (int)elementos.size(); i++)
    {
        archivo << "  Elemento " << (i + 1) << ": " << elementos[i].idCliente
                << " (peso=" << elementos[i].peso
                << ", valor=" << elementos[i].valor
                << ", ratio=" << fixed << setprecision(2) << elementos[i].razon << ")\n";
    }

    archivo << "\n";
    archivo << "TABLA COMPARATIVA:\n";
    archivo << "Enfoque        | Solicitudes | Valor | Optimo?\n";
    archivo << "-" << string(50, '-') << "\n";

    archivo << "Codicioso      | ";
    for (int idx : contraejemplo.indicesCodicioso)
        archivo << elementos[idx].idCliente << " ";
    if (contraejemplo.indicesCodicioso.empty())
        archivo << "(ninguna)";
    archivo << " | " << contraejemplo.valorCodicioso << " | ";
    archivo << (contraejemplo.encontrado ? "No" : "Si") << "\n";

    archivo << "DP (Mochila)   | ";
    for (int idx : contraejemplo.indicesOptimo)
        archivo << elementos[idx].idCliente << " ";
    if (contraejemplo.indicesOptimo.empty())
        archivo << "(ninguna)";
    archivo << " | " << contraejemplo.valorOptimo << " | Si\n";

    if (contraejemplo.encontrado)
    {
        archivo << "\n* CONTRAEJEMPLO ENCONTRADO: Codicioso falla porque selecciona\n";
        archivo << "  elementos con mejor ratio pero no maximiza el valor total.\n";
    }
    else
    {
        archivo << "\nNota: Con estos 3 elementos, ambos enfoques dan el mismo resultado.\n";
        archivo << "Para un contraejemplo clásico teórico:\n";
        archivo << "  A(w=60, v=100, r~1.67), B(w=60, v=100, r~1.67), C(w=50, v=90, r=1.8)\n";
        archivo << "  Cap=100: Codicioso->C(90), DP->mejor opcion\n";
    }

    // === ACTIVIDAD 3: Reconstrucción ===
    archivo << "\n\nACTIVIDAD 3: RECONSTRUCCION DE LA SOLUCION (BACKTRACKING)\n";
    archivo << "-" << string(55, '-') << "\n\n";

    archivo << "Algoritmo: Comienza en dp[" << elementos.size() << "][" << C << "] = " << resultado.valorOptimo << "\n";
    archivo << "Retrocede verificando si cada elemento fue incluido en la solucion.\n";
    archivo << "Recupera los indices en orden:\n";
    for (int i = 0; i < (int)resultado.indicesSeleccionados.size(); i++)
    {
        if (i > 0)
            archivo << ", ";
        archivo << "[" << resultado.indicesSeleccionados[i] << "]";
    }
    archivo << "\n";

    // === ACTIVIDAD 4: Analisis de complejidad ===
    archivo << "\n\nACTIVIDAD 4: ANALISIS DE COMPLEJIDAD\n";
    archivo << "-" << string(35, '-') << "\n\n";

    archivo << "COMPLEJIDAD DE TIEMPO: O(n*W)\n";
    archivo << "  - Construccion tabla DP: n*W operaciones\n";
    archivo << "  - Backtracking: O(n)\n";
    archivo << "  - Total: O(" << elementos.size() << "*" << C << ") = O("
            << (elementos.size() * C) << ")\n\n";

    archivo << "COMPLEJIDAD DE ESPACIO: O(n*W)\n";
    archivo << "  - Tabla DP: (n+1)*(W+1) celdas = " << ((elementos.size() + 1) * (C + 1)) << " celdas\n";
    archivo << "  - Espacio auxiliar: O(n)\n";
    archivo << "  - Total: O(" << (elementos.size() * C) << ")\n\n";

    archivo << "PSEUDOPOLINOMIALIDAD:\n";
    archivo << "  Este algoritmo es PSEUDOPOLINOMIAL (no polinomial estricto).\n";
    archivo << "  Razon: W es el VALOR de capacidad, no su tamano en bits.\n";
    archivo << "  - Si W esta acotado por polinomio(entrada): Polinomial\n";
    archivo << "  - Si W crece exponencialmente: Tiempo exponencial real\n";
    archivo << "  - Ejemplo: W = 2^(numero de solicitudes) -> exponencial\n\n";

    archivo << "CONCLUSION: O(n*W) es pseudopolinomial. Para NP-hard, es muy eficiente.\n";

    archivo << "\n\nTiempo de ejecucion: " << fixed << setprecision(3) << duracion.count() << " ms\n";
    archivo << "===============================================\n";

    archivo.close();

    cout << "\n* Archivo generado: results/asignacion_bw.txt\n";
    cout << "Tiempo de ejecucion: " << fixed << setprecision(3) << duracion.count() << " ms\n";
}
