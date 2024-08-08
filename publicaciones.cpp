#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// Clase Publicacion
class Publicacion {
public:
    Publicacion(std::string correo, std::string contenido, std::string fecha, std::string hora)
        : correo_(correo), contenido_(contenido), fecha_(fecha), hora_(hora) {}

    std::string getCorreo() const { return correo_; }
    void setCorreo(const std::string& correo) { correo_ = correo; }

    std::string getContenido() const { return contenido_; }
    void setContenido(const std::string& contenido) { contenido_ = contenido; }

    std::string getFecha() const { return fecha_; }
    void setFecha(const std::string& fecha) { fecha_ = fecha; }

    std::string getHora() const { return hora_; }
    void setHora(const std::string& hora) { hora_ = hora; }

private:
    std::string correo_;
    std::string contenido_;
    std::string fecha_;
    std::string hora_;

    friend class ListaPublicaciones;
};

struct NodoPublicacion {
    Publicacion publicacion;
    NodoPublicacion* siguiente;

    NodoPublicacion(const Publicacion& publicacion) : publicacion(publicacion), siguiente(nullptr) {}
};

// Lista simplemente enlazada para Publicaciones
class ListaPublicaciones {
public:
    ListaPublicaciones() : cabeza(nullptr) {}

    ~ListaPublicaciones() {
        NodoPublicacion* actual = cabeza;
        while (actual != nullptr) {
            NodoPublicacion* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    void agregarPublicacion(const Publicacion& publicacion) {
        NodoPublicacion* nuevoNodo = new NodoPublicacion(publicacion);
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            NodoPublicacion* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
    }

    void mostrarPublicaciones() const {
        NodoPublicacion* actual = cabeza;
        while (actual != nullptr) {
            std::cout << "Correo: " << actual->publicacion.getCorreo() << std::endl;
            std::cout << "Contenido: " << actual->publicacion.getContenido() << std::endl;
            std::cout << "Fecha: " << actual->publicacion.getFecha() << std::endl;
            std::cout << "Hora: " << actual->publicacion.getHora() << std::endl;
            std::cout << "----------------------------------" << std::endl;
            actual = actual->siguiente;
        }
    }

    static ListaPublicaciones cargarPublicacionesDesdeJson(const std::string& nombreArchivo) {
        ListaPublicaciones listaPublicaciones;
        std::ifstream archivo(nombreArchivo);

        if (!archivo.is_open()) {
            std::cerr << "Error al abrir el archivo JSON." << std::endl;
            return listaPublicaciones;
        }

        std::string linea;
        std::string correo, contenido, fecha, hora;

        while (std::getline(archivo, linea)) {
            linea.erase(std::remove(linea.begin(), linea.end(), ' '), linea.end());
            linea.erase(std::remove(linea.begin(), linea.end(), '\"'), linea.end());

            if (linea.find("correo:") != std::string::npos) {
                correo = linea.substr(linea.find(":") + 1);
            } else if (linea.find("contenido:") != std::string::npos) {
                contenido = linea.substr(linea.find(":") + 1);
            } else if (linea.find("fecha:") != std::string::npos) {
                fecha = linea.substr(linea.find(":") + 1);
            } else if (linea.find("hora:") != std::string::npos) {
                hora = linea.substr(linea.find(":") + 1);
                Publicacion publicacion(correo, contenido, fecha, hora);
                listaPublicaciones.agregarPublicacion(publicacion);
            }
        }

        archivo.close();
        return listaPublicaciones;
    }

private:
    NodoPublicacion* cabeza;
};

