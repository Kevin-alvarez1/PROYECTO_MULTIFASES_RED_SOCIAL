#ifndef GRAFO_NO_DIRIGIDO_H
#define GRAFO_NO_DIRIGIDO_H

#include <iostream>
#include <string>

class Nodo {
public:
    std::string nombre;
    Nodo** vecinos;  // Arreglo dinámico de punteros a nodos vecinos
    int numVecinos;  // Cantidad de vecinos actuales
    int capacidadVecinos;  // Capacidad actual del arreglo de vecinos

    Nodo(const std::string& nombre);
    ~Nodo();

    void agregarVecino(Nodo* vecino);
    bool tieneVecino(Nodo* nodo) const;
};

class GrafoNoDirigido {
private:
    Nodo** nodos;  // Arreglo dinámico de punteros a nodos
    int numNodos;  // Cantidad de nodos actuales en el grafo
    int capacidadNodos;  // Capacidad actual del arreglo de nodos

    Nodo* encontrarNodo(const std::string& nombre) const;
    void redimensionarNodos();
    void generarArchivoDOT(const std::string& nombreArchivo) const;

public:
    // Constructor y destructor
    GrafoNoDirigido();
    ~GrafoNoDirigido();

    // Métodos públicos
    bool existeNombre(const std::string& nombre) const;
    void insertarNombre(const std::string& nombre);
    void insertarRelacion(const std::string& nombre1, const std::string& nombre2);
    std::string* obtenerAmigos(const std::string& nombre, int& cantidadAmigos) const;
    void mostrarGrafo() const;
    void generarPNG(const std::string& nombreArchivo) const;
};

#endif // GRAFO_NO_DIRIGIDO_H
