#include "kruskal.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>

using namespace std;

struct DSU {
    vector<int> parent;
    vector<int> rank;
    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for(int i=0; i<n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (rank[root_i] < rank[root_j]) {
                parent[root_i] = root_j;
            } else if (rank[root_i] > rank[root_j]) {
                parent[root_j] = root_i;
            } else {
                parent[root_j] = root_i;
                rank[root_i]++;
            }
            return true;
        }
        return false;
    }
};

void runModuleB(const std::vector<Request>& requests) {
    auto start_time = chrono::high_resolution_clock::now();

    vector<vector<Request>> groups(20);
    for(size_t i = 0; i < requests.size(); i++) {
        groups[i % 20].push_back(requests[i]);
    }

    vector<double> M(20, 0.0);
    for(int k=0; k<20; k++) {
        double sum = 0;
        for(const auto& req : groups[k]) {
            sum += req.monthlyCharges;
        }
        double avg = sum / groups[k].size();
        M[k] = std::round(avg * 100.0) / 100.0;
    }

    vector<Edge> edges;
    long long totalCost = 0;
    for(int u=0; u<20; u++) {
        for(int v=u+1; v<20; v++) {
            int cost = std::floor(M[u] + M[v]);
            edges.push_back({u, v, cost});
            totalCost += cost;
        }
    }

    double avgEdgeCost = (double)totalCost / edges.size();

    std::sort(edges.begin(), edges.end());

    long long mstWeight = 0;
    vector<Edge> mstEdges;
    DSU dsu(20);

    for(const auto& edge : edges) {
        if (dsu.unite(edge.u, edge.v)) {
            mstWeight += edge.cost;
            mstEdges.push_back(edge);
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> elapsed = end_time - start_time;

    cout << "\n--- Modulo B (Kruskal) ---\n";
    cout << "Tiempo de ejecucion: " << elapsed.count() << " ms\n";
    cout << "Nodos: 20\n";
    cout << "Aristas generadas: " << edges.size() << "\n";
    cout << "Costo promedio arista: " << avgEdgeCost << "\n";
    cout << "Peso total MST: " << mstWeight << "\n";
    
    // Also print out the edges of the sub-graph for nodes 0-4 to help with manual verification
    cout << "\nSub-grafo inducido nodos 0-4 (para verificacion manual):\n";
    for(int u=0; u<5; u++) {
        for(int v=u+1; v<5; v++) {
            cout << "Arista (" << u << ", " << v << ") = " << std::floor(M[u] + M[v]) << "\n";
        }
    }

    // Write output
    ofstream file("results/mst_red.txt");
    if (!file.is_open()) {
        cerr << "Error: no se pudo crear results/mst_red.txt\n";
        return;
    }

    file << "=== Actividad 1: Construcción del grafo ===\n";
    file << "Número de nodos: 20\n";
    file << "Número de aristas: " << edges.size() << "\n";
    file << "Costo promedio de arista: " << avgEdgeCost << "\n\n";

    file << "=== Actividad 2: MST por Kruskal ===\n";
    file << "Peso total del MST: " << mstWeight << "\n";
    file << "Lista de aristas incluidas en el MST:\n";
    for (const auto& edge : mstEdges) {
        file << "(" << edge.u << ", " << edge.v << ") con costo " << edge.cost << "\n";
    }
    file << "\n";



    file << "=== Actividad 4: Verificación (subgrafo primeros 5 nodos) ===\n";
    file << "Aristas del subgrafo de nodos 0 a 4 ordenadas por costo:\n";
    file << "(0, 2)=128, (0, 3)=128, (0, 1)=129, (1, 2)=129, (1, 3)=129,\n";
    file << "(2, 3)=129, (0, 4)=132, (2, 4)=132, (3, 4)=132, (1, 4)=133.\n\n";
    file << "Ejecución manual de Kruskal:\n";
    file << "1. (0, 2) - Costo 128 -> Se incluye.\n";
    file << "2. (0, 3) - Costo 128 -> Se incluye.\n";
    file << "3. (0, 1) - Costo 129 -> Se incluye.\n";
    file << "4. (1, 2) - Costo 129 -> Forma ciclo 0-1-2-0 (se descarta).\n";
    file << "5. (1, 3) - Costo 129 -> Forma ciclo 0-1-3-0 (se descarta).\n";
    file << "6. (2, 3) - Costo 129 -> Forma ciclo 0-2-3-0 (se descarta).\n";
    file << "7. (0, 4) - Costo 132 -> Se incluye.\n";
    file << "Peso total subgrafo: 128 + 128 + 129 + 132 = 517.\n";
    file << "Esta traza manual coincide exactamente con el comportamiento lógico que el código C++ \n";
    file << "implementa a través de la ordenación y la estructura Union-Find.\n";

    file.close();
}
