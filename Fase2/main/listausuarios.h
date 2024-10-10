// ListaUsuarios.h
#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include "NodoAVL.h"
#include <string>
#include "usuario.h"
#include <iostream>
#include <fstream>
#include "json.hpp"


class NodoUsuario {
public:
    Usuario usuario;
    NodoUsuario* siguiente;

    NodoUsuario(const Usuario& user) : usuario(user), siguiente(nullptr) {}
};

class ListaUsuarios {
private:

    NodoAVL* raiz;
    NodoAVL* balancear(NodoAVL* nodo);
    NodoUsuario* cabeza;

    int altura(NodoAVL* nodo) const;
    int balance(NodoAVL* nodo) const;
    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);
    NodoAVL* insertar(NodoAVL* nodo, const Usuario& usuario);
    NodoAVL* buscar(NodoAVL* nodo, const std::string& correo) const;
    NodoAVL* eliminar(NodoAVL* nodo, const std::string& correo);
    NodoAVL* minimoNodo(NodoAVL* nodo);
    void eliminarArbol(NodoAVL* nodo);
    int obtenerAltura(NodoAVL* nodo);
    int obtenerBalance(NodoAVL* nodo);
    void generateDotRec(NodoAVL* nodo, std::ofstream& file) const;
    void agregarUsuarioEnLista(Usuario usuario);

public:
    ListaUsuarios();
    ~ListaUsuarios();

    void agregarUsuario(const Usuario& usuario);
    Usuario* buscarUsuarioPorCorreo(const std::string& correo);
    void borrarUsuarioPorCorreo(const std::string& correo);
    void cargarUsuariosDesdeJson(const std::string& nombreArchivo);
    bool buscarUsuarioPorCorreoyContrasena(const std::string& correo, const std::string& contrasena) const;
    bool usuarioDuplicado(const std::string &correo) const;
    Usuario mostrarDatosPorCorreo(const std::string& correo) const;
    void preOrder(NodoAVL* nodo, NodoUsuario*& lista);
    void inOrder(NodoAVL* nodo, NodoUsuario*& lista);
    void postOrder(NodoAVL* nodo, NodoUsuario*& lista);
    void generarLista(NodoAVL* nodo, NodoUsuario*& lista, const std::string& tipoOrden);

    NodoUsuario* obtenerUsuariosEnOrden(const std::string& tipoOrden);
    void generateDot(const std::string& filename) const;
    NodoUsuario* obtenerPrimero() {
        return cabeza;
    }
};

#endif // LISTAUSUARIOS_H
