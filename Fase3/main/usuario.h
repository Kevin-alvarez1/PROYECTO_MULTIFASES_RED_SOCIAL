#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <sstream>

class Usuario
{
public:
    Usuario(const std::string &nombre, const std::string &apellido, const std::string &fecha_de_nacimiento,
            const std::string &correo, const std::string &contrasena);

    std::string getNombre() const;
    std::string getApellido() const;
    std::string getFechaDeNacimiento() const;
    std::string getCorreo() const;
    std::string getContrasena() const;

    void setNombre(const std::string &nombre);
    void setApellido(const std::string &apellido);
    void setFechaDeNacimiento(const std::string &fecha_de_nacimiento);
    void setCorreo(const std::string &correo);
    void setContrasena(const std::string &contrasena);
    void mostrarInformacion() const;

    std::string serializar() const;
    static Usuario deserializar(const std::string& datos) {
        std::istringstream stream(datos);
        std::string nombre, apellido, fecha_nacimiento, correo, contrasena;

        std::getline(stream, nombre, ',');
        std::getline(stream, apellido, ',');
        std::getline(stream, fecha_nacimiento, ',');
        std::getline(stream, correo, ',');
        std::getline(stream, contrasena, ',');

        return Usuario(nombre, apellido, fecha_nacimiento, correo, contrasena);
    }
private:
    std::string nombre_;
    std::string apellido_;
    std::string fecha_de_nacimiento_;
    std::string correo_;
    std::string contrasena_;
};

#endif // USUARIO_H
