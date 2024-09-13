#ifndef LISTA_SOLICITUDES_H
#define LISTA_SOLICITUDES_H

#include "solicitud.h"

class ListaSolicitudes {
private:
    struct NodoSolicitud {
        Solicitud solicitud;
        NodoSolicitud* siguiente;

        NodoSolicitud(const Solicitud &solicitud);
    };

    NodoSolicitud* cabeza;

public:
    ListaSolicitudes();
    ~ListaSolicitudes();

    void agregarSolicitud(const Solicitud &solicitud);
    void eliminarSolicitud(const std::string &emisor, const std::string &receptor);
    void mostrarSolicitudes() const;
    void cargarRelacionesDesdeJson(const std::string &filename);
    void enviarSolicitud(const std::string &emisor, const std::string &receptor);

};

#endif // LISTA_SOLICITUDES_H
