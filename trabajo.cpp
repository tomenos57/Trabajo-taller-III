#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

string* v = new string[4];       
int cantidad_actual = 0;         
int capacidad_maxima = 4;        
double porcentaje_overhead = 0.2; 
int indice_letras[258];

size_t CalcularMemoriaRAM() {
    size_t memoria_total = 0;

    memoria_total += sizeof(v) + sizeof(cantidad_actual) + sizeof(capacidad_maxima) + sizeof(porcentaje_overhead);
    memoria_total += sizeof(indice_letras); 

    memoria_total += capacidad_maxima * sizeof(string);

    for (int i = 0; i < cantidad_actual; i++) {
        memoria_total += v[i].capacity(); 
    }

    return memoria_total;
}

void Actualizar_arreglo_indice_letras() {
    for (int i = 0; i < 258; i++) indice_letras[i] = cantidad_actual;
    for (int i = cantidad_actual - 1; i >= 0; i--) {
        if (!v[i].empty()) {
            unsigned char letra = v[i][0];
            indice_letras[letra] = i;
        }
    }
    for (int i = 255; i >= 0; i--) {
        if (indice_letras[i] == cantidad_actual && i < 255) {
            indice_letras[i] = indice_letras[i + 1];
        }
    }
}

int BuscarPosicionParaInsertar(string nueva_palabra) {
    if (cantidad_actual == 0) return 0;
    unsigned char letra_inicial = nueva_palabra[0];
    int l = indice_letras[letra_inicial];
    int r = indice_letras[letra_inicial + 1] - 1;

    if (l > r) return l;

    while (l <= r) {
        int medio = (l + r) / 2;
        if (v[medio] >= nueva_palabra) r = medio - 1;
        else l = medio + 1;
    }
    return l;
}

int Buscar1(string palabra_buscada) {
    char letra_inicial = palabra_buscada[0];
    
    int l = indice_letras[letra_inicial];
    int r = indice_letras[letra_inicial + 1] - 1;

    while (l <= r) {
        int medio = (l + r) / 2;
        
        if (v[medio] == palabra_buscada) {
            return medio; 
        }
        if (v[medio] < palabra_buscada) {
            l = medio + 1;
        } else {
            r = medio - 1;
        }
    }
    return -1; 
}

void insertar1(string nueva_palabra) {
    if (cantidad_actual == capacidad_maxima) {
   
        int nueva_capacidad = capacidad_maxima * (1 + porcentaje_overhead);
        
        if (nueva_capacidad == capacidad_maxima) {
            nueva_capacidad++; 
        }
        
        capacidad_maxima = nueva_capacidad;
        string* nuevo_v = new string[capacidad_maxima];

        for (int i = 0; i < cantidad_actual; i++) {
            nuevo_v[i] = v[i];
        }

        delete[] v;
        v = nuevo_v;
    }

    int posicion = BuscarPosicionParaInsertar(nueva_palabra);

    for (int i = cantidad_actual; i >= posicion + 1; i--) {
        v[i] = v[i - 1];
    }

    v[posicion] = nueva_palabra;
    cantidad_actual++;
    
    Actualizar_arreglo_indice_letras();
}

int main(){
    ifstream archivo("D1.txt"); //dependiendo de que diccionario se busque
    string palabra;

    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo D1.txt" << endl;
        return 1; 
    }

    cout << "CARGANDO DICCIONARIO" << endl;
    
    auto inicio_carga = high_resolution_clock::now();
    
    while (archivo >> palabra) {
        insertar1(palabra);
    }
    
    auto fin_carga = high_resolution_clock::now();
    auto duracion_carga = duration_cast<milliseconds>(fin_carga - inicio_carga);
    archivo.close(); 

    cout << "Total de palabras: " << cantidad_actual << endl;
    cout << "Tiempo total de insercion: " << duracion_carga.count() << " ms" << endl;
    
    size_t ram_bytes = CalcularMemoriaRAM();
    cout << "Espacio estimado en RAM: " << ram_bytes << " bytes (" << ram_bytes / 1024.0 << " KB)" << endl;
    cout << "----------------------------\n" << endl;

    string palabra_a_buscar; 
    cout << "Ingrese la palabra a buscar: ";
    cin >> palabra_a_buscar;

    auto inicio_busqueda = high_resolution_clock::now();
    
    int pos = Buscar1(palabra_a_buscar);
    
    auto fin_busqueda = high_resolution_clock::now();
    auto duracion_busqueda = duration_cast<microseconds>(fin_busqueda - inicio_busqueda);

    if (pos != -1) {
        cout << "Resultado: Encontrada en el indice [" << pos+1 << "]" << endl;
    } else {
        cout << "Resultado: No existe en el diccionario." << endl;
    }
    
    cout << "Tiempo de busqueda: " << duracion_busqueda.count() << " microsegundos." << endl;

    return 0;
}