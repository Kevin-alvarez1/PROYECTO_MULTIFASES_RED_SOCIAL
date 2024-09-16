#ifndef COMENTARIO_H
#define COMENTARIO_H

#include <string>

class Comentario {
public:
    Comentario(const std::string& correo, const std::string& comentario, const std::string& fecha, const std::string& hora);

    std::string getCorreo() const;
    std::string getComentario() const;
    std::string getFecha() const;
    std::string getHora() const;

private:
    std::string correo_;
    std::string comentario_;
    std::string fecha_;
    std::string hora_;
};

#endif // COMENTARIO_H
