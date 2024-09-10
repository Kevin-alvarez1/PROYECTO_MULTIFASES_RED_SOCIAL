#ifndef NODOAVL_H
#define NODOAVL_H

#include "Usuario.h"

class NodoAVL {
public:
    Usuario usuario;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    // Constructor
    NodoAVL(const Usuario& user);

private:
         // Otros métodos privados si los necesitas
};

#endif // NODOAVL_H
