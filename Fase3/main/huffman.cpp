#include "huffman.h"
#include <fstream>
#include <bitset>
#include <cassert>

Huffman huffman;

Huffman::Huffman() : raiz_(nullptr) {}

Huffman::~Huffman() {
    // Implementar liberación de memoria si es necesario
}

// Método para calcular frecuencias de los caracteres en los datos
void Huffman::calcularFrecuencias(const char* datos, int longitud, int frecuencias[256]) {
    memset(frecuencias, 0, 256 * sizeof(int)); // Inicializar frecuencias a 0
    for (int i = 0; i < longitud; ++i) {
        frecuencias[(unsigned char)datos[i]]++; // Incrementar frecuencia del carácter
    }
}

// Método para construir el árbol de Huffman
NodoHuffman* Huffman::construirArbol(int frecuencias[256]) {
    NodoPrioridad* cola = nullptr;

    // Insertar nodos en la cola de prioridad
    for (int i = 0; i < 256; ++i) {
        if (frecuencias[i] > 0) { // Solo insertamos caracteres con frecuencia mayor a 0
            insertarCola(cola, new NodoHuffman((char)i, frecuencias[i]));
        }
    }

    // Construir el árbol de Huffman
    while (cola && cola->siguiente) {
        NodoHuffman* izquierda = extraerMin(cola);
        NodoHuffman* derecha = extraerMin(cola);
        NodoHuffman* padre = new NodoHuffman('\0', izquierda->frecuencia + derecha->frecuencia);
        padre->izquierda = izquierda;
        padre->derecha = derecha;
        insertarCola(cola, padre);
    }

    return extraerMin(cola);  // Retornar la raíz del árbol
}

// Método para generar códigos a partir del árbol de Huffman
void Huffman::generarCodigos(NodoHuffman* raiz, char* codigoActual, int profundidad, char* codigos[256]) {
    if (!raiz) return;

    if (!raiz->izquierda && !raiz->derecha) {
        codigoActual[profundidad] = '\0'; // Terminar la cadena de código
        codigos[(unsigned char)raiz->caracter] = new char[profundidad + 1];
        strcpy(codigos[(unsigned char)raiz->caracter], codigoActual);
    }

    // Recursión a la izquierda y derecha
    codigoActual[profundidad] = '0';
    generarCodigos(raiz->izquierda, codigoActual, profundidad + 1, codigos);
    codigoActual[profundidad] = '1';
    generarCodigos(raiz->derecha, codigoActual, profundidad + 1, codigos);
}

// Método para codificar los datos usando los códigos generados
void Huffman::codificarDatos(const char* datos, int longitud, char* codigos[256], char*& datosCodificados, int& longitudCodificados) {
    std::string resultado;
    for (int i = 0; i < longitud; ++i) {
        resultado += codigos[(unsigned char)datos[i]];
    }

    longitudCodificados = resultado.size();
    datosCodificados = new char[longitudCodificados + 1];
    strcpy(datosCodificados, resultado.c_str());
}

// Método para escribir los bits codificados en un archivo
void Huffman::escribirBits(const char* datosCodificados, int longitudCodificados, const char* nombreArchivoSalida) {
    std::ofstream archivoSalida(nombreArchivoSalida, std::ios::binary);
    if (!archivoSalida.is_open()) {
        std::cerr << "Error al abrir el archivo de salida." << std::endl;
        return;
    }

    // Escribir los datos codificados (esto es solo un ejemplo)
    archivoSalida.write(datosCodificados, longitudCodificados);
    archivoSalida.close(); // Asegúrate de cerrar el archivo
}


// Método para comprimir datos desde un archivo
bool Huffman::comprimir(const char* nombreArchivoEntrada, const char* nombreArchivoSalida) {
    std::ifstream archivoEntrada(nombreArchivoEntrada, std::ios::binary);
    if (!archivoEntrada.is_open()) {
        std::cerr << "Error al abrir el archivo de entrada." << std::endl;
        return false;
    }

    // Leer todos los datos del archivo
    archivoEntrada.seekg(0, std::ios::end);
    int longitud = archivoEntrada.tellg();
    archivoEntrada.seekg(0, std::ios::beg);

    char* datos = new char[longitud];
    archivoEntrada.read(datos, longitud);
    archivoEntrada.close();

    // Calcular frecuencias de los caracteres
    int frecuencias[256] = {0};  // Inicializar a cero
    calcularFrecuencias(datos, longitud, frecuencias);

    // Construir el árbol de Huffman
    NodoHuffman* arbolHuffman = construirArbol(frecuencias);

    // Generar códigos
    char* codigos[256] = {nullptr};
    char codigoActual[256]; // Suficiente para códigos de longitud máxima 256
    generarCodigos(arbolHuffman, codigoActual, 0, codigos);

    // Codificar los datos
    char* datosCodificados = nullptr; // Inicializar en nullptr
    int longitudCodificados = 0;
    codificarDatos(datos, longitud, codigos, datosCodificados, longitudCodificados);

    // Escribir los bits codificados en un archivo
    escribirBits(datosCodificados, longitudCodificados, nombreArchivoSalida);

    // Liberar memoria del árbol de Huffman y datos
    delete[] datos;
    for (int i = 0; i < 256; ++i) {
        delete[] codigos[i]; // Liberar cada código
    }
    delete[] datosCodificados;

    return true;
}

// Métodos para manejar la cola de prioridad
void Huffman::insertarCola(NodoPrioridad*& cabeza, NodoHuffman* nodo) {
    NodoPrioridad* nuevoNodo = new NodoPrioridad();
    nuevoNodo->nodo = nodo;
    nuevoNodo->siguiente = nullptr;

    if (!cabeza || cabeza->nodo->frecuencia > nodo->frecuencia) {
        nuevoNodo->siguiente = cabeza;
        cabeza = nuevoNodo;
    } else {
        NodoPrioridad* actual = cabeza;
        while (actual->siguiente && actual->siguiente->nodo->frecuencia <= nodo->frecuencia) {
            actual = actual->siguiente;
        }
        nuevoNodo->siguiente = actual->siguiente;
        actual->siguiente = nuevoNodo;
    }
}

NodoHuffman* Huffman::extraerMin(NodoPrioridad*& cabeza) {
    if (!cabeza) return nullptr;
    NodoHuffman* minNodo = cabeza->nodo;
    NodoPrioridad* temp = cabeza;
    cabeza = cabeza->siguiente;
    delete temp; // Liberar el nodo de la cola
    return minNodo;
}

// Método para descomprimir datos
void Huffman::descomprimir(const char* nombreArchivoEntrada, NodoHuffman* arbolHuffman, const char* nombreArchivoSalida) {
    std::ifstream archivoEntrada(nombreArchivoEntrada, std::ios::binary);
    if (!archivoEntrada.is_open()) {
        std::cerr << "Error al abrir el archivo de entrada." << std::endl;
        return;
    }

    std::ofstream archivoSalida(nombreArchivoSalida, std::ios::binary);
    if (!archivoSalida.is_open()) {
        std::cerr << "Error al abrir el archivo de salida." << std::endl;
        return;
    }

    // Aquí iría la lógica para leer los bits del archivo y reconstruir los datos usando el árbol de Huffman

    archivoEntrada.close();
    archivoSalida.close();
}
