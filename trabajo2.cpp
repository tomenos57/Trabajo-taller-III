#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

struct NodoNivel {
    string Clave;
    NodoNivel* Next;
    NodoNivel* Prev;
    NodoNivel* Abajo;

    NodoNivel(string c) {
        Clave = c; Next = nullptr; Prev = nullptr; Abajo = nullptr;
    }
};

vector<NodoNivel*> HEADER_cabezas;
vector<NodoNivel*> TAIL_nodos;
int contador_nodos_L1 = 0;
int parametro_k = 8;

size_t CalcularMemoriaRAM() {
    size_t memoria_total = sizeof(HEADER_cabezas) + sizeof(TAIL_nodos) + sizeof(contador_nodos_L1) + sizeof(parametro_k);
    
    for (int i = 0; i < HEADER_cabezas.size(); i++) {
        NodoNivel* cur = HEADER_cabezas[i];
        while (cur != nullptr) {
            memoria_total += sizeof(NodoNivel) + cur->Clave.capacity();
            cur = cur->Next;
        }
    }
    return memoria_total;
}

int comparar(string a, string b) {
    if (a == b) return 0;
    if (a > b) return 1;
    return -1;
}

bool Buscar2(string objetivo) {
    if (HEADER_cabezas.empty()) return false;
    
    int nivel_actual = HEADER_cabezas.size() - 1;
    NodoNivel* cur = HEADER_cabezas[nivel_actual];
    if (!cur) return false;

    do {
        while (cur->Next != nullptr && comparar(objetivo, cur->Next->Clave) >= 0) {
            cur = cur->Next;
        }

        if (cur->Abajo == nullptr) {
            while (cur != nullptr && comparar(objetivo, cur->Clave) != 0) {
                cur = cur->Next;
            }
            return (cur != nullptr);
        }
        cur = cur->Abajo;
    } while (true);
}

NodoNivel* InsertarEnNivel(int nivel, string clave, NodoNivel* nodo_abajo) {
    NodoNivel* n = new NodoNivel(clave);
    n->Abajo = nodo_abajo;

    NodoNivel* h = HEADER_cabezas[nivel];
    if (!h) {
        HEADER_cabezas[nivel] = n;
        TAIL_nodos[nivel] = n;
        return n;
    }

    if (TAIL_nodos[nivel] != nullptr && clave > TAIL_nodos[nivel]->Clave) {
        TAIL_nodos[nivel]->Next = n;
        n->Prev = TAIL_nodos[nivel];
        TAIL_nodos[nivel] = n;
        return n;
    }

    NodoNivel* p = h;
    while (p != nullptr && comparar(clave, p->Clave) > 0) {
        p = p->Next;
    }

    if (p == h) {
        n->Next = h;
        h->Prev = n;
        HEADER_cabezas[nivel] = n;
    } else if (p == nullptr) {
        TAIL_nodos[nivel]->Next = n;
        n->Prev = TAIL_nodos[nivel];
        TAIL_nodos[nivel] = n;
    } else {
        n->Next = p;
        n->Prev = p->Prev;
        p->Prev->Next = n;
        p->Prev = n;
    }
    return n;
}

void Insertar2(string nueva_palabra) {
    if (HEADER_cabezas.empty()) {
        HEADER_cabezas.push_back(nullptr);
        TAIL_nodos.push_back(nullptr);
    }

    if (Buscar2(nueva_palabra)) return;

    NodoNivel* nodo_inferior = InsertarEnNivel(0, nueva_palabra, nullptr);
    contador_nodos_L1++;

    int temp_count = contador_nodos_L1;
    int nivel = 1;

    while (temp_count % parametro_k == 0) {
        if (nivel >= HEADER_cabezas.size()) {
            HEADER_cabezas.push_back(nullptr);
            TAIL_nodos.push_back(nullptr);

            NodoNivel* primera_palabra = HEADER_cabezas[0];
            NodoNivel* copia_primero = new NodoNivel(primera_palabra->Clave);
            copia_primero->Abajo = HEADER_cabezas[nivel - 1]; 
            
            HEADER_cabezas[nivel] = copia_primero;
            TAIL_nodos[nivel] = copia_primero;
        }

        nodo_inferior = InsertarEnNivel(nivel, nueva_palabra, nodo_inferior);
        nivel++;
        temp_count /= parametro_k;
    }
}

int main() {
    ifstream archivo("D1.txt"); 
    string palabra;

    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo D1.txt" << endl;
        return 1; 
    }

    cout << "--- CARGANDO GRILLA DE LISTAS ---" << endl;
    cout << "Usando parametro k = " << parametro_k << endl;
    
    auto inicio_carga = high_resolution_clock::now();
    
    while (archivo >> palabra) {
        Insertar2(palabra);
    }
    
    auto fin_carga = high_resolution_clock::now();
    auto duracion_carga = duration_cast<milliseconds>(fin_carga - inicio_carga);
    archivo.close(); 

    cout << "Niveles creados dinamicamente: " << HEADER_cabezas.size() << endl;
    cout << "Total de palabras en Nivel Base (L1): " << contador_nodos_L1 << endl;
    cout << "Tiempo total de insercion: " << duracion_carga.count() << " ms" << endl;
    
    size_t ram_bytes = CalcularMemoriaRAM();
    cout << "Espacio estimado en RAM: " << ram_bytes << " bytes (" << ram_bytes / 1024.0 / 1024.0 << " MB)" << endl;
    cout << "----------------------------\n" << endl;

    string palabra_a_buscar; 
    cout << "Ingrese la palabra a buscar: ";
    cin >> palabra_a_buscar;

    auto inicio_busqueda = high_resolution_clock::now();
    
    bool encontrada = Buscar2(palabra_a_buscar);
    
    auto fin_busqueda = high_resolution_clock::now();
    auto duracion_busqueda = duration_cast<microseconds>(fin_busqueda - inicio_busqueda);

    if (encontrada) {
        cout << "Resultado: ¡Palabra encontrada!" << endl;
    } else {
        cout << "Resultado: No existe en el diccionario." << endl;
    }
    
    cout << "Tiempo de busqueda: " << duracion_busqueda.count() << " microsegundos." << endl;

    return 0;
}