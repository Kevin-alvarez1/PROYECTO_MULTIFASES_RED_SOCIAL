#ifndef LISTA_SOLICITUDES_H
#define LISTA_SOLICITUDES_H

#include "solicitud.h"
#include <string>
#include <vector>
#include "grafo_no_dirigido.h"
#include "arbolabb.h"

extern GrafoNoDirigido grafoNoDirigido;

class ListaSolicitudes {
private:
    class NodoSolicitud {
    public:
        Solicitud solicitud;
        NodoSolicitud* siguiente;

        NodoSolicitud(const Solicitud &solicitud);
    };

    NodoSolicitud* cabeza;

public:
    ListaSolicitudes();
    ~ListaSolicitudes();
    bool existeSolicitudEnEstado(const std::string& emisor, const std::string& receptor, const std::string& estado) const;

    void agregarSolicitud(const Solicitud &solicitud);
    void eliminarSolicitud(const std::string &emisor, const std::string &receptor);
    void mostrarSolicitudes() const;
    void cargarRelacionesDesdeJson(const std::string &filename);
    void enviarSolicitud(const std::string &emisor, const std::string &receptor);
    std::vector<std::string> obtenerSolicitudesEnviadas(const std::string &correoEmisor) const;
    bool existeSolicitudEnEstado(const std::string &emisor, const std::string &receptor, const std::string &estado);
    std::vector<Solicitud> obtenerSolicitudesPorReceptor(const std::string &correoReceptor) const;
    void buscarYApilarPendientes(const std::string &correo, const ListaSolicitudes &listaSolicitudes);
    bool actualizarEstadoSolicitud(const std::string& emisor, const std::string& receptor, const std::string& nuevoEstado);
    void agregarRelacionesAceptadasAMatriz(GrafoNoDirigido &grafoNoDirigido);
    std::vector<Solicitud> getSolicitudes() const {
        std::vector<Solicitud> solicitudes; // Vector para almacenar las solicitudes
        NodoSolicitud* actual = cabeza; // Comenzamos desde la cabeza
        while (actual != nullptr) { // Recorremos la lista
            solicitudes.push_back(actual->solicitud); // Agregamos la solicitud al vector
            actual = actual->siguiente; // Pasamos al siguiente nodo
        }
        return solicitudes; // Retornamos todas las solicitudes
    }
    void guardarSolicitudesEnviadas() const;
};

#endif // LISTA_SOLICITUDES_H
