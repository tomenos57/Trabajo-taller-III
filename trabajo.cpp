#include <iostream>
#include <string>
using namespace std;

// VArreglos del arreglo dinamico 
string* v = new string[4];       
int cantidad_actual = 0;         
int capacidad_maxima = 4;        
double porcentaje_overhead = 0.2; // Cuánto crece el arreglo al llenarse
int indice_letras[258];          
int main(){

}

//metodo de arreglo dinamico
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
        capacidad_maxima = capacidad_maxima * (1 + porcentaje_overhead);
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

void Eliminar(string palabra) {
    int posicion = Buscar1(palabra);

    if (posicion != -1) {
        for (int i = posicion; i <= cantidad_actual - 2; i++) {
            v[i] = v[i + 1];
        }
        cantidad_actual--;
        
        Actualizar_arreglo_indice_letras();
    }
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


