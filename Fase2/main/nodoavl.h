#ifndef NODOAVL_H
#define NODOAVL_H

#include "usuario.h" // O cualquier dependencia que sea necesaria

class NodoAVL {
public:
    Usuario usuario;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(const Usuario& user);  // Constructor

    // Puedes añadir más métodos si es necesario
};

#endif // NODOAVL_H
