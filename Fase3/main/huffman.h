#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <cstdio>
#include <cstring>

// Estructura del nodo para el árbol de Huffman
struct NodoHuffman {
    char caracter;
    int frecuencia;
    NodoHuffman* izquierda;
    NodoHuffman* derecha;

    NodoHuffman(char c, int f) : caracter(c), frecuencia(f), izquierda(nullptr), derecha(nullptr) {}
};

// Estructura para la cola de prioridad manual
struct NodoPrioridad {
    NodoHuffman* nodo;
    NodoPrioridad* siguiente;
};

// Clase Huffman para compresión y descompresión
class Huffman {
public:
    // Constructor y destructor
    Huffman();
    ~Huffman();

    // Métodos para compresión
    void calcularFrecuencias(const char* datos, int longitud, int frecuencias[256]);
    NodoHuffman* construirArbol(int frecuencias[256]);
    void generarCodigos(NodoHuffman* raiz, char* codigoActual, int profundidad, char* codigos[256]);
    void codificarDatos(const char* datos, int longitud, char* codigos[256], char*& datosCodificados, int& longitudCodificados);
    void escribirBits(const char* datosCodificados, int longitudCodificados, const char* nombreArchivoSalida);
    bool comprimir(const char* nombreArchivoEntrada, const char* nombreArchivoSalida);

    // Métodos para descompresión
    void descomprimir(const char* nombreArchivoEntrada, NodoHuffman* arbolHuffman, const char* nombreArchivoSalida);

    // Métodos para manejar la cola de prioridad
    void insertarCola(NodoPrioridad*& cabeza, NodoHuffman* nodo);
    NodoHuffman* extraerMin(NodoPrioridad*& cabeza);

private:
    NodoHuffman* raiz_;  // Nodo raíz del árbol de Huffman
};

#endif // HUFFMAN_H
