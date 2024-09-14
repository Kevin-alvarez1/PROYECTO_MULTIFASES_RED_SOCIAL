#include "lista_solicitudes.h"
#include "solicitud.h"
#include "PilaReceptor.h"
#include <iostream>
#include "json.hpp"
#include <fstream>

// Aquí se debe tener un método para obtener la pila del receptor
PilaReceptor& obtenerPilaReceptor(const std::string &correoReceptor);  // Debes implementar esta función

// Constructor y Destructor
ListaSolicitudes::ListaSolicitudes() : cabeza(nullptr) {}

ListaSolicitudes::~ListaSolicitudes() {
    NodoSolicitud* actual = cabeza;
    while (actual != nullptr) {
        NodoSolicitud* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
}

ListaSolicitudes::NodoSolicitud::NodoSolicitud(const Solicitud &solicitud)
    : solicitud(solicitud), siguiente(nullptr) {}

// Función para agregar solicitud
void ListaSolicitudes::agregarSolicitud(const Solicitud &solicitud) {
    NodoSolicitud* nuevoNodo = new NodoSolicitud(solicitud);
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
}

// Función para eliminar solicitud
void ListaSolicitudes::eliminarSolicitud(const std::string &emisor, const std::string &receptor) {
    NodoSolicitud* actual = cabeza;
    NodoSolicitud* anterior = nullptr;

    while (actual != nullptr && (actual->solicitud.getEmisor() != emisor || actual->solicitud.getReceptor() != receptor)) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual != nullptr) {
        if (anterior != nullptr) {
            anterior->siguiente = actual->siguiente;
        } else {
            cabeza = actual->siguiente;
        }
        delete actual;
    }
}

// Función para mostrar solicitudes
void ListaSolicitudes::mostrarSolicitudes() const {
    NodoSolicitud* actual = cabeza;
    while (actual != nullptr) {
        std::cout << "Emisor: " << actual->solicitud.getEmisor()
        << ", Receptor: " << actual->solicitud.getReceptor()
        << ", Estado: " << actual->solicitud.getEstado() << std::endl;
        actual = actual->siguiente;
    }
}

// Función para enviar solicitud
void ListaSolicitudes::enviarSolicitud(const std::string &emisor, const std::string &receptor) {
    // Crear una nueva solicitud con estado "PENDIENTE"
    Solicitud nuevaSolicitud(emisor, receptor, "PENDIENTE");

    // Verificar si ya existe una solicitud entre el mismo emisor y receptor
    NodoSolicitud* actual = cabeza;
    while (actual != nullptr) {
        if (actual->solicitud.getEmisor() == emisor && actual->solicitud.getReceptor() == receptor) {
            std::cerr << "Ya existe una solicitud entre " << emisor << " y " << receptor << std::endl;
            return;
        }
        actual = actual->siguiente;
    }

    // Agregar la nueva solicitud a la lista de solicitudes
    agregarSolicitud(nuevaSolicitud);

    // Agregar la solicitud a la pila de solicitudes recibidas del receptor
    PilaReceptor& pilaReceptor = obtenerPilaReceptor(receptor);
    Receptor nuevaSolicitudReceptor(emisor, receptor, "PENDIENTE");
    pilaReceptor.push(nuevaSolicitudReceptor);

    std::cout << "Solicitud enviada de " << emisor << " a " << receptor << " con estado PENDIENTE." << std::endl;
}

// Función para verificar solicitudes en estado
bool ListaSolicitudes::existeSolicitudEnEstado(const std::string &emisor, const std::string &receptor, const std::string &estado) {
    NodoSolicitud* actual = cabeza;

    while (actual != nullptr) {
        const Solicitud& solicitud = actual->solicitud;
        if ((solicitud.getEmisor() == emisor && solicitud.getReceptor() == receptor) ||
            (solicitud.getEmisor() == receptor && solicitud.getReceptor() == emisor)) {
            if (solicitud.getEstado() == estado) {
                return true;
            }
        }
        actual = actual->siguiente;
    }

    return false;
}

void ListaSolicitudes::cargarRelacionesDesdeJson(const std::string &nombreArchivo)
{
    std::ifstream archivo(nombreArchivo);
    if (archivo.is_open())
    {
        nlohmann::json jsonData;
        archivo >> jsonData;
        archivo.close();

        for (const auto &item : jsonData)
        {
            std::string emisor = item["emisor"];
            std::string receptor = item["receptor"];
            std::string estado = item["estado"];

            Solicitud nuevaSolicitud(emisor, receptor, estado);
            agregarSolicitud(nuevaSolicitud);
        }

        std::cout << "Depuración: Finalizada la carga de relaciones desde " << nombreArchivo << std::endl;
        std::cout << "===============================\n";
        std::cout << "Relaciones cargadas exitosamente.\n";
        std::cout << "===============================\n";
    }

    else
    {
        std::cerr << "Error al abrir el archivo JSON." << std::endl;
    }
}


// Obtener solicitudes enviadas por un emisor
std::vector<std::string> ListaSolicitudes::obtenerSolicitudesEnviadas(const std::string &correoEmisor) const {
    std::vector<std::string> receptores;
    NodoSolicitud* actual = cabeza;

    while (actual != nullptr) {
        if (actual->solicitud.getEmisor() == correoEmisor) {
            receptores.push_back(actual->solicitud.getReceptor());
        }
        actual = actual->siguiente;
    }

    return receptores;
}
