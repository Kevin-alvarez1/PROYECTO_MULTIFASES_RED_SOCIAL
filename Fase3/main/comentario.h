#ifndef COMENTARIO_H
#define COMENTARIO_H

#include <string>
#include "json.hpp"

class Comentario {
public:
    // Constructor
    Comentario() : contenido_(""), idPublicacion_(0), correo_(""), fecha_(""), hora_("") {}

    Comentario(const std::string& contenido, int idPublicacion, const std::string& correo, const std::string& fecha, const std::string& hora);

    // Getters
    std::string getCorreo() const;
    std::string getComentario() const;
    std::string getFecha() const;
    std::string getHora() const;
    int getIdPublicacion() const;
    bool esVacio() const {
        return contenido_.empty();
    }

    std::string toJSON() const {
        nlohmann::json jsonData;
        jsonData["correo"] = this->correo_;
        jsonData["contenido"] = this->contenido_;
        jsonData["fecha"] = this->fecha_;
        jsonData["hora"] = this->hora_;
        return jsonData.dump();
    }
private:
    std::string contenido_;
    std::string correo_;
    int idPublicacion_;
    std::string fecha_;
    std::string hora_;
};

#endif // COMENTARIO_H
