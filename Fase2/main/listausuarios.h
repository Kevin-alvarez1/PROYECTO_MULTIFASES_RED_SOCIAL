// ListaUsuarios.h
#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include "NodoAVL.h"
#include <string>
#include "usuario.h"
#include <iostream>
#include <fstream>
#include "json.hpp"

class ListaUsuarios {
private:

    NodoAVL* raiz;

    // Métodos privados para el manejo del árbol AVL
    int altura(NodoAVL* nodo) const;
    int balance(NodoAVL* nodo) const;
    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);
    NodoAVL* insertar(NodoAVL* nodo, const Usuario& usuario);
    NodoAVL* buscar(NodoAVL* nodo, const std::string& correo) const;
    NodoAVL* eliminar(NodoAVL* nodo, const std::string& correo);
    NodoAVL* minimoNodo(NodoAVL* nodo);
    void eliminarArbol(NodoAVL* nodo);

public:
    ListaUsuarios();
    ~ListaUsuarios();

    void agregarUsuario(const Usuario& usuario);
    Usuario* buscarUsuarioPorCorreo(const std::string& correo) const;
    void borrarUsuarioPorCorreo(const std::string& correo);
    void cargarUsuariosDesdeJson(const std::string& nombreArchivo);
    void generateDot(const std::string& filename) const;
    bool buscarUsuarioPorCorreoyContrasena(const std::string& correo, const std::string& contrasena) const;
    void renderGraphviz(const std::string& dotFilename, const std::string& imageFilename) const;
    bool usuarioDuplicado(const std::string &correo) const;
    Usuario mostrarDatosPorCorreo(const std::string &correo) const;


};

#endif // LISTAUSUARIOS_H
