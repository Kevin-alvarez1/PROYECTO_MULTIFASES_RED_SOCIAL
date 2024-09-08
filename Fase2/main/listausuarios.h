#ifndef LISTAUSUARIOS_H
#define LISTAUSUARIOS_H

#include <iostream>
#include <fstream>
#include <string>
#include "usuario.h"
#include "json.hpp"

class Nodo
{
public:
    Usuario usuario;
    Nodo *siguiente;

    Nodo(const Usuario &usuario) : usuario(usuario), siguiente(nullptr)
    {
        std::cout << "Depuración: Nodo creado para usuario con correo: " << usuario.getCorreo() << std::endl;
    }
};

class ListaUsuarios
{
public:
    ListaUsuarios();
    ~ListaUsuarios();

    void agregarUsuario(const Usuario &usuario);
    bool usuarioDuplicado(const std::string &correo) const;
    void cargarUsuariosDesdeJson(const std::string &nombreArchivo);
    void mostrarDatosPorCorreo(const std::string &correo) const;
    void borrarUsuarioPorCorreo(const std::string &correo);
    void generateDot(const std::string &filename) const;
    void renderGraphviz(const std::string &dotFilename, const std::string &imageFilename) const;
    bool buscarUsuarioPorCorreoyContrasena(const std::string &correo, const std::string &contrasena) const;

private:
    Nodo *cabeza;
    Nodo *cola;

    std::string limpiarCadena(const std::string &str);
};

#endif // LISTAUSUARIOS_H
