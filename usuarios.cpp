#include <iostream>
#include <string>

class Usuario {
public:
    // Constructor
    Usuario(std::string nombre, std::string apellido, std::string fecha_de_nacimiento, std::string correo, std::string contrasena) : nombre_(nombre), apellido_(apellido), fecha_de_nacimiento_(fecha_de_nacimiento), correo_(correo), contrasena_(contrasena) {}

    // Métodos getter y setter para nombre
    std::string getNombre() const {
        return nombre_;
    }

    void setNombre(const std::string& nombre) {
        nombre_ = nombre;
    }

    // Métodos getter y setter para apellido
    std::string getApellido() const {
        return apellido_;
    }

    void setApellido(const std::string& apellido) {
        apellido_ = apellido;
    }

    // Métodos getter y setter para fecha_de_nacimiento
    std::string getFechaDeNacimiento() const {
        return fecha_de_nacimiento_;
    }

    void setFechaDeNacimiento(const std::string& fecha_de_nacimiento) {
        fecha_de_nacimiento_ = fecha_de_nacimiento;
    }

    // Métodos getter y setter para correo
    std::string getCorreo() const {
        return correo_;
    }

    void setCorreo(const std::string& correo) {
        correo_ = correo;
    }

    // Métodos getter y setter para contrasena
    std::string getContrasena() const {
        return contrasena_;
    }

    void setContrasena(const std::string& contrasena) {
        contrasena_ = contrasena;
    }

private:
    // Miembros de datos
    std::string nombre_;
    std::string apellido_;
    std::string fecha_de_nacimiento_;
    std::string correo_;
    std::string contrasena_;
};