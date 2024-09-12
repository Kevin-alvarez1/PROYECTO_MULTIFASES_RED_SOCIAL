#ifndef PUBLICACION_H
#define PUBLICACION_H

#include <string>
#include <vector>
#include "Comentario.h"

class Publicacion {
public:
    Publicacion(int id, const std::string &correo, const std::string &contenido, const std::string &fecha, const std::string &hora);

    int getId() const;
    std::string getCorreo() const;
    std::string getContenido() const;
    std::string getFecha() const;
    std::string getHora() const;
    std::vector<Comentario> getComentarios() const;

    void setId(int id);
    void setCorreo(const std::string &correo);
    void setContenido(const std::string &contenido);
    void setFecha(const std::string &fecha);
    void setHora(const std::string &hora);
    void agregarComentario(const Comentario &comentario);
    void mostrarComentarios() const;
    void mostrarInformacion() const;

private:
    int id_;
    std::string correo_;
    std::string contenido_;
    std::string fecha_;
    std::string hora_;
    std::vector<Comentario> comentarios_;
};

#endif // PUBLICACION_H
