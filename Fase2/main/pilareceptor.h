#ifndef PILARECEPTOR_H
#define PILARECEPTOR_H

#include "receptor.h"
#include <iostream>
#include <unordered_map>
#include <string>

class NodoReceptor {
public:
    Receptor receptor;
    NodoReceptor* siguiente;

    NodoReceptor(const Receptor& receptor_);
};

class PilaReceptor {
private:
    NodoReceptor* cima;

public:
    PilaReceptor();
    ~PilaReceptor();
    PilaReceptor(const PilaReceptor& otra);
    PilaReceptor& operator=(const PilaReceptor& otra);

    // Métodos de la pila
    bool estaVacia() const;
    void push(const Receptor& receptor);
    Receptor pop();
    Receptor peek() const;
    void mostrarPila() const;
    void actualizarEstadoSolicitud(const std::string& emisor, const std::string& receptor, const std::string& nuevoEstado);

};

// Declaración de la función global
PilaReceptor& obtenerPilaReceptor(const std::string &correoReceptor);

#endif // PILARECEPTOR_H
