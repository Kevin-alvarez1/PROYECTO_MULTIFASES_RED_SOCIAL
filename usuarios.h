#ifndef USUARIOS_H
#define USUARIOS_H

#include <string>
#include <vector>

class Usuario {
public:
    Usuario(std::string nombre, std::string apellido, std::string fecha_de_nacimiento, std::string correo, std::string contrasena);

    std::string getNombre() const;
    std::string getApellido() const;
    std::string getFechaDeNacimiento() const;
    std::string getCorreo() const;
    std::string getContrasena() const;

private:
    std::string nombre_;
    std::string apellido_;
    std::string fecha_de_nacimiento_;
    std::string correo_;
    std::string contrasena_;

    friend class ListaUsuarios;
};

struct Nodo {
    Usuario usuario;
    Nodo* siguiente;

    Nodo(const Usuario& usuario);
};

class ListaUsuarios {
public:
    ListaUsuarios();
    ~ListaUsuarios();

    void agregarUsuario(const Usuario& usuario);
    bool usuarioDuplicado(const std::string& correo) const;
    Nodo* buscarUsuarioPorCorreoYContrasena(const std::string& correo, const std::string& contrasena) const;
    void mostrarUsuarios() const;

    static ListaUsuarios cargarUsuariosDesdeJson(const std::string& nombreArchivo);

private:
    Nodo* cabeza;
};

#endif // USUARIOS_H
