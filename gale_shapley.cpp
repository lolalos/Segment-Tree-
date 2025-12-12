// gale_shapley.cpp
// Algoritmo de Gale-Shapley para el Problema de Emparejamiento Estable
// Ejercicio 2: Emparejar 10 hombres y 10 mujeres según sus preferencias

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

const int N = 10; // Número de hombres y mujeres

// Nombres de hombres y mujeres para visualización
string hombres[] = {"Carlos", "Diego", "Eduardo", "Fernando", "Gabriel", 
                    "Hernán", "Ignacio", "Javier", "Kevin", "Luis"};
string mujeres[] = {"Ana", "Beatriz", "Carla", "Diana", "Elena", 
                   "Fernanda", "Gloria", "Helena", "Isabel", "Julia"};

// Preferencias de cada hombre (0-indexed)
// Cada fila: preferencias de un hombre, ordenadas de más a menos preferida
int pref_hombres[N][N] = {
    {2, 5, 1, 8, 3, 7, 0, 9, 6, 4},  // Carlos
    {7, 3, 2, 0, 9, 1, 8, 5, 4, 6},  // Diego
    {5, 2, 8, 1, 4, 6, 7, 0, 3, 9},  // Eduardo
    {1, 4, 6, 0, 8, 3, 5, 2, 9, 7},  // Fernando
    {9, 7, 3, 5, 0, 2, 6, 1, 8, 4},  // Gabriel
    {0, 8, 6, 2, 4, 5, 1, 9, 7, 3},  // Hernán
    {3, 1, 9, 7, 5, 0, 4, 8, 2, 6},  // Ignacio
    {4, 2, 5, 9, 1, 6, 0, 3, 8, 7},  // Javier
    {8, 0, 3, 7, 9, 4, 5, 2, 1, 6},  // Kevin
    {6, 9, 4, 1, 3, 8, 2, 7, 5, 0}   // Luis
};

// Preferencias de cada mujer (0-indexed)
// Cada fila: preferencias de una mujer, ordenadas de más a menos preferido
int pref_mujeres[N][N] = {
    {5, 7, 1, 8, 3, 9, 0, 4, 6, 2},  // Ana
    {3, 6, 9, 0, 2, 7, 1, 5, 8, 4},  // Beatriz
    {1, 7, 4, 0, 9, 5, 2, 8, 3, 6},  // Carla
    {8, 4, 6, 1, 9, 0, 7, 5, 2, 3},  // Diana
    {9, 3, 7, 5, 2, 1, 0, 8, 4, 6},  // Elena
    {2, 0, 4, 6, 3, 8, 9, 1, 7, 5},  // Fernanda
    {4, 3, 9, 5, 8, 2, 0, 1, 7, 6},  // Gloria
    {1, 4, 0, 9, 6, 8, 2, 5, 3, 7},  // Helena
    {0, 5, 3, 2, 9, 7, 4, 6, 1, 8},  // Isabel
    {6, 1, 4, 7, 3, 0, 8, 5, 2, 9}   // Julia
};

// Ranking inverso: para cada mujer, posición del hombre en su lista de preferencias
int ranking_mujeres[N][N];

void construir_ranking() {
    for (int m = 0; m < N; m++) {
        for (int pos = 0; pos < N; pos++) {
            int hombre = pref_mujeres[m][pos];
            ranking_mujeres[m][hombre] = pos;
        }
    }
}

void imprimir_preferencias() {
    cout << "\n===============================================================================\n";
    cout << "                    PREFERENCIAS DE LOS HOMBRES\n";
    cout << "===============================================================================\n\n";
    
    for (int h = 0; h < N; h++) {
        cout << setw(10) << left << hombres[h] << ": ";
        for (int i = 0; i < N; i++) {
            cout << "#" << setw(2) << right << (i+1) << " " << setw(10) << left << mujeres[pref_hombres[h][i]];
            if (i < N - 1) cout << " | ";
        }
        cout << "\n";
    }
    
    cout << "\n===============================================================================\n";
    cout << "                    PREFERENCIAS DE LAS MUJERES\n";
    cout << "===============================================================================\n\n";
    
    for (int m = 0; m < N; m++) {
        cout << setw(10) << left << mujeres[m] << ": ";
        for (int i = 0; i < N; i++) {
            cout << "#" << setw(2) << right << (i+1) << " " << setw(10) << left << hombres[pref_mujeres[m][i]];
            if (i < N - 1) cout << " | ";
        }
        cout << "\n";
    }
}

pair<vector<int>, int> gale_shapley_detallado() {
    vector<int> comprometida_con(N, -1);  // comprometida_con[m] = h si mujer m está con hombre h
    vector<int> pareja_de(N, -1);         // pareja_de[h] = m si hombre h está con mujer m
    vector<int> siguiente_propuesta(N, 0); // índice de la siguiente mujer a la que propondrá
    
    queue<int> hombres_libres;
    for (int h = 0; h < N; h++) {
        hombres_libres.push(h);
    }
    
    int iteraciones = 0;
    cout << "\n╔════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║            EJECUCIÓN DEL ALGORITMO GALE-SHAPLEY                       ║\n";
    cout << "╚════════════════════════════════════════════════════════════════════════╝\n\n";
    
    while (!hombres_libres.empty()) {
        int h = hombres_libres.front();
        hombres_libres.pop();
        
        if (siguiente_propuesta[h] >= N) continue;
        
        int m = pref_hombres[h][siguiente_propuesta[h]];
        siguiente_propuesta[h]++;
        iteraciones++;
        
        cout << "Iteración " << iteraciones << ":\n";
        cout << "  " << hombres[h] << " propone a " << mujeres[m] << "\n";
        
        if (comprometida_con[m] == -1) {
            // Mujer está libre
            comprometida_con[m] = h;
            pareja_de[h] = m;
            cout << "  → " << mujeres[m] << " acepta (estaba libre)\n";
        } else {
            int h_actual = comprometida_con[m];
            if (ranking_mujeres[m][h] < ranking_mujeres[m][h_actual]) {
                // Prefiere al nuevo
                cout << "  → " << mujeres[m] << " prefiere a " << hombres[h] 
                     << " sobre " << hombres[h_actual] << "\n";
                cout << "  → " << hombres[h_actual] << " queda libre\n";
                
                comprometida_con[m] = h;
                pareja_de[h] = m;
                pareja_de[h_actual] = -1;
                hombres_libres.push(h_actual);
            } else {
                // Prefiere al actual
                cout << "  → " << mujeres[m] << " rechaza (prefiere a " 
                     << hombres[h_actual] << ")\n";
                hombres_libres.push(h);
            }
        }
        cout << "\n";
    }
    
    return {pareja_de, iteraciones};
}

void verificar_estabilidad(const vector<int>& pareja_de) {
    cout << "╔════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║              VERIFICACIÓN DE ESTABILIDAD                              ║\n";
    cout << "╚════════════════════════════════════════════════════════════════════════╝\n\n";
    
    bool estable = true;
    int parejas_bloqueantes = 0;
    
    // Verificar si existe alguna pareja bloqueante
    for (int h = 0; h < N; h++) {
        int m_actual = pareja_de[h];
        
        // Para cada mujer que h prefiere sobre su pareja actual
        for (int i = 0; i < N; i++) {
            int m = pref_hombres[h][i];
            
            if (m == m_actual) break; // Ya llegó a su pareja actual
            
            // h prefiere a m sobre m_actual
            int h_pareja_m = -1;
            for (int j = 0; j < N; j++) {
                if (pareja_de[j] == m) {
                    h_pareja_m = j;
                    break;
                }
            }
            
            // Si m prefiere a h sobre su pareja actual, hay inestabilidad
            if (ranking_mujeres[m][h] < ranking_mujeres[m][h_pareja_m]) {
                estable = false;
                parejas_bloqueantes++;
                cout << "⚠ Pareja bloqueante encontrada:\n";
                cout << "  " << hombres[h] << " (con " << mujeres[m_actual] 
                     << ") prefiere a " << mujeres[m] << "\n";
                cout << "  " << mujeres[m] << " (con " << hombres[h_pareja_m] 
                     << ") prefiere a " << hombres[h] << "\n\n";
            }
        }
    }
    
    if (estable) {
        cout << "✓ El emparejamiento es ESTABLE\n";
        cout << "  No existen parejas bloqueantes\n\n";
    } else {
        cout << "✗ El emparejamiento es INESTABLE\n";
        cout << "  Se encontraron " << parejas_bloqueantes << " parejas bloqueantes\n\n";
    }
}

void imprimir_estadisticas(const vector<int>& pareja_de) {
    cout << "╔════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                    ESTADÍSTICAS DEL EMPAREJAMIENTO                    ║\n";
    cout << "╚════════════════════════════════════════════════════════════════════════╝\n\n";
    
    // Satisfacción de los hombres
    cout << "SATISFACCIÓN DE LOS HOMBRES (posición de su pareja en su lista):\n";
    double suma_h = 0;
    for (int h = 0; h < N; h++) {
        int m = pareja_de[h];
        int pos = 0;
        for (int i = 0; i < N; i++) {
            if (pref_hombres[h][i] == m) {
                pos = i;
                break;
            }
        }
        suma_h += pos + 1;
        cout << "  " << setw(10) << left << hombres[h] << " obtuvo opción #" 
             << (pos + 1) << " (" << mujeres[m] << ")\n";
    }
    cout << "  Promedio: " << fixed << setprecision(2) << (suma_h / N) << "\n\n";
    
    // Satisfacción de las mujeres
    cout << "SATISFACCIÓN DE LAS MUJERES (posición de su pareja en su lista):\n";
    double suma_m = 0;
    for (int m = 0; m < N; m++) {
        int h = -1;
        for (int i = 0; i < N; i++) {
            if (pareja_de[i] == m) {
                h = i;
                break;
            }
        }
        int pos = ranking_mujeres[m][h];
        suma_m += pos + 1;
        cout << "  " << setw(10) << left << mujeres[m] << " obtuvo opción #" 
             << (pos + 1) << " (" << hombres[h] << ")\n";
    }
    cout << "  Promedio: " << fixed << setprecision(2) << (suma_m / N) << "\n\n";
    
    cout << "RESUMEN:\n";
    cout << "  • Los hombres obtienen en promedio su opción #" 
         << fixed << setprecision(2) << (suma_h / N) << "\n";
    cout << "  • Las mujeres obtienen en promedio su opción #" 
         << fixed << setprecision(2) << (suma_m / N) << "\n";
    cout << "  • El algoritmo favorece a quien propone (hombres en este caso)\n\n";
}

int main() {
    cout << "\n╔════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║        ALGORITMO DE GALE-SHAPLEY - PROBLEMA DE EMPAREJAMIENTO        ║\n";
    cout << "║                    ESTABLE (STABLE MARRIAGE)                          ║\n";
    cout << "╚════════════════════════════════════════════════════════════════════════╝\n";
    
    construir_ranking();
    imprimir_preferencias();
    
    auto [pareja_de, iteraciones] = gale_shapley_detallado();
    
    cout << "╔════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                  EMPAREJAMIENTO FINAL                                 ║\n";
    cout << "╚════════════════════════════════════════════════════════════════════════╝\n\n";
    
    for (int h = 0; h < N; h++) {
        cout << "  " << setw(10) << left << hombres[h] << " ← → " 
             << setw(10) << left << mujeres[pareja_de[h]] << "\n";
    }
    cout << "\n  Total de iteraciones: " << iteraciones << "\n\n";
    
    verificar_estabilidad(pareja_de);
    imprimir_estadisticas(pareja_de);
    
    cout << "╔════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║                    PROPIEDADES DEL ALGORITMO                          ║\n";
    cout << "╚════════════════════════════════════════════════════════════════════════╝\n\n";
    cout << "  • Complejidad temporal: O(N²)\n";
    cout << "  • Siempre encuentra un emparejamiento estable\n";
    cout << "  • El emparejamiento es óptimo para quien propone\n";
    cout << "  • Cada persona hace como máximo N propuestas\n";
    cout << "  • El algoritmo siempre termina\n\n";
    
    return 0;
}
