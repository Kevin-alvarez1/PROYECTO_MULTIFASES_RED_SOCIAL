#include "lista_solicitudes.h"
#include "solicitud.h"
#include <iostream>
#include "json.hpp"
#include <fstream>

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

void ListaSolicitudes::agregarSolicitud(const Solicitud &solicitud) {
    NodoSolicitud* nuevoNodo = new NodoSolicitud(solicitud);
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
}

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

void ListaSolicitudes::mostrarSolicitudes() const {
    NodoSolicitud* actual = cabeza;
    while (actual != nullptr) {
        std::cout << "Emisor: " << actual->solicitud.getEmisor()
        << ", Receptor: " << actual->solicitud.getReceptor()
        << ", Estado: " << actual->solicitud.getEstado() << std::endl;
        actual = actual->siguiente;
    }
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

    // Agregar la nueva solicitud a la cola
    agregarSolicitud(nuevaSolicitud);

    std::cout << "Solicitud enviada de " << emisor << " a " << receptor << " con estado PENDIENTE." << std::endl;
}

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
