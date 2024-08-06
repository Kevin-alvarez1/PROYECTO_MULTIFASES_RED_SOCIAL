#include <iostream>
#include <string>

class Admin {
public:
    // Constructor
    Admin(std::string adminNombre, std::string adminApellido, std::string adminFechaDeNacimiento, std::string adminCorreo, std::string adminContrasena) : nombre_(adminNombre), apellido_(adminApellido), fecha_de_nacimiento_(adminFechaDeNacimiento), correo_(adminCorreo), contrasena_(adminContrasena) {}
    void mostrarInformacion() const {
        std::cout << "Nombre: " << nombre_ << ", Apellido: " << apellido_ << ", Fecha de nacimiento: " << fecha_de_nacimiento_ << ", Correo: " << correo_ << ", Contrasena: " << contrasena_ << std::endl;
    }    
    // Métodos getter y setter para nombre
    std::string getNombre() const {
        return nombre_;
    }

    void setNombre(const std::string& adminNombre) {
        nombre_ = adminNombre;
    }

    // Métodos getter y setter para apellido
    std::string getApellido() const {
        return apellido_;
    }

    void setApellido(const std::string& adminApellido) {
        apellido_ = adminApellido;
    }

    // Métodos getter y setter para fecha_de_nacimiento
    std::string getFechaDeNacimiento() const {
        return fecha_de_nacimiento_;
    }

    void setFechaDeNacimiento(const std::string& adminFechaDeNacimiento) {
        fecha_de_nacimiento_ = adminFechaDeNacimiento;
    }

    // Métodos getter y setter para correo
    std::string getCorreo() const {
        return correo_;
    }

    void setCorreo(const std::string& adminCorreo) {
        correo_ = adminCorreo;
    }

    // Métodos getter y setter para contrasena
    std::string getContrasena() const {
        return contrasena_;
    }

    void setContrasena(const std::string& adminContrasena) {
        contrasena_ = adminContrasena;
    }

private:
    // Miembros de datos
    std::string nombre_;
    std::string apellido_;
    std::string fecha_de_nacimiento_;
    std::string correo_;
    std::string contrasena_;
};
int main() {
    // Crear un objeto de la clase Persona
    Admin adminObj("Admin","Admin","16/06/2003","admin@gmail.com","EDD2S2024");
    
    // Mostrar información del objeto
    adminObj.mostrarInformacion();


    return 0;
}