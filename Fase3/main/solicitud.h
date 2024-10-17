#ifndef SOLICITUD_H
#define SOLICITUD_H

#include <string>
#include <iostream>
#include "grafo_no_dirigido.h"
extern GrafoNoDirigido grafoNoDirigido;
class Solicitud {
public:
    Solicitud(const std::string &emisor, const std::string &receptor, const std::string &estado);

    std::string getEmisor() const;
    void setEmisor(const std::string &emisor);

    std::string getReceptor() const;
    void setReceptor(const std::string &receptor);

    std::string getEstado() const;
    void setEstado(const std::string &estado);
    static Solicitud deserializar(const std::string& linea) {
        // Aquí asumimos que la línea tiene el formato "emisor,receptor,estado"
        size_t pos1 = linea.find(',');
        size_t pos2 = linea.find(',', pos1 + 1);

        std::string emisor = linea.substr(0, pos1);
        std::string receptor = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string estado = linea.substr(pos2 + 1);
        return Solicitud(emisor, receptor, estado);
    }
private:
    std::string emisor_;
    std::string receptor_;
    std::string estado_;
};

#endif // SOLICITUD_H
