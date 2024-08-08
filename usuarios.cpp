#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

class Usuario {
public:
    Usuario(std::string nombre, std::string apellido, std::string fecha_de_nacimiento, std::string correo, std::string contrasena)
        : nombre_(nombre), apellido_(apellido), fecha_de_nacimiento_(fecha_de_nacimiento), correo_(correo), contrasena_(contrasena) {}

    std::string getNombre() const { return nombre_; }
    std::string getApellido() const { return apellido_; }
    std::string getFechaDeNacimiento() const { return fecha_de_nacimiento_; }
    std::string getCorreo() const { return correo_; }
    std::string getContrasena() const { return contrasena_; }

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

    Nodo(const Usuario& usuario) : usuario(usuario), siguiente(nullptr) {}
};

class ListaUsuarios {
public:
    ListaUsuarios() : cabeza(nullptr) {}

    ~ListaUsuarios() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    void agregarUsuario(const Usuario& usuario) {
        if (usuarioDuplicado(usuario.getCorreo())) {
            std::cerr << "Usuario con correo " << usuario.getCorreo() << " ya existe." << std::endl;
            return;
        }

        Nodo* nuevoNodo = new Nodo(usuario);
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            Nodo* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
    }

    bool usuarioDuplicado(const std::string& correo) const {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            if (actual->usuario.getCorreo() == correo) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    Nodo* buscarUsuarioPorCorreoYContrasena(const std::string& correo, const std::string& contrasena) const {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            if (actual->usuario.getCorreo() == correo && actual->usuario.getContrasena() == contrasena) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    void mostrarUsuarios() const {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            std::cout << "Nombre: " << actual->usuario.getNombre() << std::endl;
            std::cout << "Apellido: " << actual->usuario.getApellido() << std::endl;
            std::cout << "Fecha de Nacimiento: " << actual->usuario.getFechaDeNacimiento() << std::endl;
            std::cout << "Correo: " << actual->usuario.getCorreo() << std::endl;
            std::cout << "Contraseña: " << actual->usuario.getContrasena() << std::endl;
            std::cout << "----------------------------------" << std::endl;
            actual = actual->siguiente;
        }
    }

    static ListaUsuarios cargarUsuariosDesdeJson(const std::string& nombreArchivo) {
        ListaUsuarios listaUsuarios;
        std::ifstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            std::cerr << "Error al abrir el archivo JSON." << std::endl;
            return listaUsuarios;
        }

        std::string linea;
        std::string nombre, apellido, fecha_de_nacimiento, correo, contrasena;

        while (std::getline(archivo, linea)) {
            linea.erase(std::remove(linea.begin(), linea.end(), ' '), linea.end());
            linea.erase(std::remove(linea.begin(), linea.end(), '\"'), linea.end());

            if (linea.find("nombre:") != std::string::npos) {
                nombre = linea.substr(linea.find(":") + 1);
            } else if (linea.find("apellido:") != std::string::npos) {
                apellido = linea.substr(linea.find(":") + 1);
            } else if (linea.find("fecha_de_nacimiento:") != std::string::npos) {
                fecha_de_nacimiento = linea.substr(linea.find(":") + 1);
            } else if (linea.find("correo:") != std::string::npos) {
                correo = linea.substr(linea.find(":") + 1);
            } else if (linea.find("contrasena:") != std::string::npos) {
                contrasena = linea.substr(linea.find(":") + 1);
                Usuario usuario(nombre, apellido, fecha_de_nacimiento, correo, contrasena);
                listaUsuarios.agregarUsuario(usuario);
            }
        }

        archivo.close();
        return listaUsuarios;
    }

private:
    Nodo* cabeza;
};
