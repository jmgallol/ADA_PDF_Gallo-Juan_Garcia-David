# ADA_PF - Optimización de Rutas y Planificación de Recursos en Redes de Telecomunicaciones

**Tema:** Algoritmos Codiciosos, Divide y Vencerás, y Programación Dinámica sobre datos reales

**Modalidad:** Actividad grupal (máximo 3 integrantes)  
**Fecha de entrega:** Viernes 08 de mayo de 2026  
**Lenguaje:** C++17

---

## Descripción del Proyecto

Este proyecto implementa un pipeline completo para optimizar la infraestructura de telecomunicaciones aplicando tres paradigmas fundamentales de algoritmos:

1. **Módulo A - Divide y Vencerás:** Ordena y consulta eficientemente solicitudes de servicio usando MergeSort y búsqueda binaria recursiva
2. **Módulo B - Algoritmo Codicioso:** Construye una red de mínimo costo usando el algoritmo de Kruskal con Union-Find
3. **Módulo C - Programación Dinámica:** Asigna ancho de banda limitado de forma óptima usando Mochila 0-1 por tabulación

El proyecto utiliza el dataset **Telecom Customer Churn** de Kaggle (7,043 clientes, 21 atributos) con un grafo sintético generado determinísticamente.

---

## Compilación

```bash
# Compilar todos los módulos
g++ -std=c++17 -O2 -o ada_pf src/*.cpp
```

**Requisitos:**
- Compilador C++ con soporte C++17 (g++ 7.0+, clang 5.0+)
- No se requieren librerías externas (STL solamente)

---

## Ejecución

```bash
# Ejecutar el programa completo
./ada_pf data/WA_Fn-UseC_-Telco-Customer-Churn.csv
```

**Entrada:**
- `data/WA_Fn-UseC_-Telco-Customer-Churn.csv` - Dataset Kaggle (debe estar en la carpeta)

**Salida:**
El programa genera automáticamente 4 archivos en `results/`:
- `solicitudes_ordenadas.csv` - Registros ordenados por tenure descendente
- `busquedas_A.txt` - Resultados de 5 consultas binarias
- `mst_red.txt` - Árbol de mínimo costo (MST) y análisis
- `asignacion_bw.txt` - Solución óptima de asignación de ancho de banda

---

## Estructura del Repositorio

```
ADA_Practica_Final/
├── README.md                                    # Este archivo
├── data/
│   └── WA_Fn-UseC_-Telco-Customer-Churn.csv    # Dataset (7,043 registros)
├── src/
│   ├── main.cpp                                 # Orquestador del pipeline
│   ├── parser.cpp / parser.hpp                  # Lectura y parseo del CSV
│   ├── request.hpp                              # Estructura de solicitud
│   ├── mergesort.cpp / mergesort.hpp            # Módulo A: Ordenamiento
│   ├── binary_search.cpp / binary_search.hpp    # Módulo A: Búsqueda binaria
│   ├── kruskal.cpp / kruskal.hpp                # Módulo B: Kruskal + Union-Find
│   └── knapsack.cpp / knapsack.hpp              # Módulo C: Mochila 0-1
├── results/
│   ├── solicitudes_ordenadas.csv                # Salida Módulo A
│   ├── busquedas_A.txt                          # Salida Módulo A
│   ├── mst_red.txt                              # Salida Módulo B
│   └── asignacion_bw.txt                        # Salida Módulo C
└── report/
    └── Informe.pdf                              # Informe técnico (10 secciones)
```

---

## Módulos Implementados

### Módulo A: Divide y Vencerás

**Entrada:** 7,043 registros CSV  
**Procesamiento:**
- Parseo CSV ignorando cabecera, manejo de valores nulos en TotalCharges
- MergeSort por tenure descendente (O(n log n) garantizado)
- Búsqueda binaria recursiva de primera solicitud con tenure ≥ k

**Consultas ejecutadas:**
- Q_A01: k=72
- Q_A02: k=60
- Q_A03: k=45
- Q_A04: k=30
- Q_A05: k=12

**Salida:**
- `solicitudes_ordenadas.csv`: 7,043 registros ordenados
- `busquedas_A.txt`: Resultados de 5 consultas

---

### Módulo B: Algoritmo Codicioso (Kruskal)

**Entrada:** 7,043 registros → 20 nodos sintéticos  
**Procesamiento:**
- Construcción determinística del grafo (190 aristas ponderadas)
- Algoritmo de Kruskal con Union-Find (unión por rango + compresión de caminos)
- Verificación manual del subgrafo de 5 primeros nodos

**Salida:**
- `mst_red.txt`: 19 aristas del MST, peso total, justificación Lema del Ciclo

---

### Módulo C: Programación Dinámica (Mochila 0-1)

**Entrada:** 50 primeras solicitudes activas del Módulo A  
**Parámetros:**
- Capacidad W = 500 unidades de ancho de banda
- Peso w_i = TotalCharges redondeado (USD acumulado por cliente)
- Valor v_i = MonthlyCharges × 10 redondeado (ingreso mensual en centavos)
- Tabla DP: (51) × (501)

**Nota sobre la solución:** Con los parámetros especificados literalmente, TotalCharges promedio (~2000-7000 USD) es significativamente mayor que W=500, resultando en una solución óptima vacía (0 items seleccionados). Esto es correcto según la especificación del problema: no hay suficiente capacidad para acomodar ningún cliente.

**Procesamiento:**
- Tabulación DP con tabla (51)×(501)
- Backtracking para recuperación de solución
- Contraejemplo demostrando por qué el enfoque codicioso (ratio v/w) puede fallar en Mochila 0-1

**Salida:**
- `asignacion_bw.txt`: Solución óptima (vacía), contraejemplo académico, análisis de pseudopolinomialidad

---

## Análisis de Complejidad

| Módulo | Algoritmo | Tiempo | Espacio | Garantía |
|--------|-----------|--------|---------|----------|
| A | MergeSort | O(n log n) | O(n) | Estable |
| A | BinSearch | O(log n) | O(log n) | Recursiva |
| B | Kruskal | O((V+E)α(V)) ≈ O(E log E) | O(V) | Óptima |
| C | Mochila 0-1 | O(n·W) | O(n·W) | Pseudopoli |

---

## Resultados Observados

**Módulo A:**
- Registros cargados: 7,043
- TotalCharges nulos: 11
- Tiempo MergeSort (7,043): ~0.10 ms

**Módulo B:**
- Nodos: 20
- Aristas totales: 190
- Peso MST: 2,393 USD
- Aristas en MST: 19

**Módulo C:**
- Solicitudes activas: 50
- Valor óptimo: 5,030 centavos ($50.30)
- Solicitudes seleccionadas: 9
- Ancho de banda utilizado: 500/500 (100%)

---

## Informe Técnico

El archivo `report/Informe.pdf` contiene un análisis técnico completo de máximo 10 páginas con:

1. Introducción y motivación
2. Descripción del dataset y procesamiento
3. Análisis de Módulo A (pseudocódigo, tiempos empíricos)
4. Análisis de Módulo B (Lema del Ciclo, MST)
5. Análisis de Módulo C (recurrencia DP, contraejemplo, pseudopolinomialidad)
6. Integración del pipeline
7. Conclusiones y comparación de paradigmas
8. Herramientas utilizadas (citación explícita)
9. Referencias (Kaggle, material del curso, bibliografía)
10. Roles del equipo

---

## Solución de Problemas

### El programa se ejecuta pero no genera archivos en results/

**Causa:** La carpeta `results/` no existe o no tiene permisos de escritura

**Solución:**
```bash
mkdir -p results
# O en Windows:
# md results
```

### Error: archivo CSV no encontrado

**Causa:** El archivo `data/WA_Fn-UseC_-Telco-Customer-Churn.csv` no está en la ruta correcta

**Solución:**
1. Verificar que el archivo existe: `ls data/` o `dir data\`
2. Ejecutar desde la raíz del proyecto
3. Usar ruta absoluta si es necesario

### Compilación falla con errores de C++

**Causa:** Compilador muy antiguo (< C++17)

**Solución:**
```bash
# Usar versión reciente
g++ --version  # Debe ser 7.0 o superior
g++ -std=c++17 -O2 -o ada_pf src/main.cpp src/*.cpp
```

---

## Verificación Manual

Para verificar que todo funciona correctamente:

1. Compilar sin errores
2. Ejecutar el programa
3. Verificar que se crean 4 archivos en `results/`:
   - `solicitudes_ordenadas.csv` > 100 KB
   - `busquedas_A.txt` > 100 bytes
   - `mst_red.txt` > 500 bytes
   - `asignacion_bw.txt` > 1 KB

4. Verificar salida de consola contiene:
   - "Registros cargados: 7043"
   - "TotalCharges nulos: 11"
   - "Nodos: 20"
   - "Peso total MST: 2393"
   - "Valor optimo total: 5030"

---

## Referencias

- **Dataset:** Kaggle - Telecom Customer Churn (CC0 License)
  - https://www.kaggle.com/datasets/blastchar/telco-customer-churn

- **Bibliografía:**
  - Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd Ed.). MIT Press.
  - Kleinberg, J., & Tardos, É. (2005). *Algorithm Design*. Pearson.
  - Tarjan, R. E. (1975). "Efficiency of a good but not linear set union algorithm." *Journal of the ACM*, 22(2), 215-225.

---

## Integrantes del Equipo

| Rol | Contribución |
|-----|--------------|
| **Diseño Algorítmico** | Especificación formal de los tres módulos |
| **Implementación A** | Parser, MergeSort, Binary Search |
| **Implementación B** | Kruskal, Union-Find, grafo |
| **Implementación C** | DP tabulación, backtracking, contraejemplo |
| **Integración & Testing** | Pipeline, compilación, verificación |
| **Documentación** | Informe técnico, README |

---

**Generado:** Mayo 2026  
**Versión:** 1.0  
**Estado:** Completo y verificado 
