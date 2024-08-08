#include <iostream>
#include <limits>
#include "publicaciones.cpp"
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

class Usuario
{
public:
    Usuario(std::string nombre, std::string apellido, std::string fecha_de_nacimiento, std::string correo, std::string contrasena)
        : nombre_(nombre), apellido_(apellido), fecha_de_nacimiento_(fecha_de_nacimiento), correo_(correo), contrasena_(contrasena) {}

    std::string getNombre() const { return nombre_; }
    std::string getApellido() const { return apellido_; }
    std::string getFechaDeNacimiento() const { return fecha_de_nacimiento_; }
    std::string getCorreo() const { return correo_; }
    std::string getContrasena() const { return contrasena_; }

private:
    std::string nombre_;
    std::string apellido_;
    std::string fecha_de_nacimiento_;
    std::string correo_;
    std::string contrasena_;

    friend class ListaUsuarios;
};

struct Nodo
{
    Usuario usuario;
    Nodo *siguiente;

    Nodo(const Usuario &usuario) : usuario(usuario), siguiente(nullptr) {}
};

class ListaUsuarios
{
public:
    ListaUsuarios() : cabeza(nullptr) {}

    ~ListaUsuarios()
    {
        Nodo *actual = cabeza;
        while (actual != nullptr)
        {
            Nodo *temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    void agregarUsuario(const Usuario &usuario)
    {
        std::cout << "Agregando usuario: " << usuario.getNombre() << std::endl;

        if (usuarioDuplicado(usuario.getCorreo()))
        {
            std::cerr << "Usuario con correo " << usuario.getCorreo() << " ya existe." << std::endl;
            return;
        }

        Nodo *nuevoNodo = new Nodo(usuario);
        if (cabeza == nullptr)
        {
            cabeza = nuevoNodo;
            std::cout << "Usuario agregado como cabeza de la lista." << std::endl;
        }
        else
        {
            Nodo *actual = cabeza;
            while (actual->siguiente != nullptr)
            {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
            std::cout << "Usuario agregado al final de la lista." << std::endl;
        }
    }

    bool usuarioDuplicado(const std::string &correo) const
    {
        Nodo *actual = cabeza;
        while (actual != nullptr)
        {
            if (actual->usuario.getCorreo() == correo)
            {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    bool buscarUsuarioPorCorreoyContrasena(const std::string &correo, const std::string &contrasena) const
    {
        Nodo *temp = cabeza;
        while (temp != nullptr)
        {
            if (temp->usuario.getCorreo() == correo && temp->usuario.getContrasena() == contrasena)
            {
                return true;
            }
            temp = temp->siguiente;
        }
        return false;
    }

static ListaUsuarios cargarUsuariosDesdeJson(const std::string &nombreArchivo)
{
    ListaUsuarios listaUsuarios;
    std::ifstream archivo(nombreArchivo);

    if (!archivo.is_open())
    {
        std::cerr << "Error al abrir el archivo JSON." << std::endl;
        return listaUsuarios;
    }

    std::string linea;
    std::vector<std::string> datos;

    while (std::getline(archivo, linea))
    {
        linea.erase(linea.find_last_not_of(" \n\r\t") + 1);
        linea.erase(0, linea.find_first_not_of(" \n\r\t"));

        if (linea.empty() || linea == "{" || linea == "}")
        {
            continue;
        }

        // Verifica si la línea contiene un par clave:valor
        if (linea.find(":") != std::string::npos)
        {
            std::size_t pos = linea.find(":");
            std::string clave = linea.substr(0, pos);
            std::string valor = linea.substr(pos + 1);

            // Elimina las comillas y los espacios adicionales de la clave y el valor
            clave.erase(std::remove(clave.begin(), clave.end(), '\"'), clave.end());
            valor.erase(std::remove(valor.begin(), valor.end(), '\"'), valor.end());
            clave.erase(std::remove(clave.begin(), clave.end(), ' '), clave.end());
            valor.erase(std::remove(valor.begin(), valor.end(), ' '), valor.end());

            // Elimina la coma final si está presente en el valor
            if (!valor.empty() && valor.back() == ',')
            {
                valor.pop_back();
            }

            // Verifica y agrega el valor al vector de datos
            if (clave == "nombre" || clave == "apellido" || clave == "fecha_de_nacimiento" || clave == "correo" || clave == "contrasena")
            {
                datos.push_back(valor);
            }

            // Procesa el usuario completo cuando se haya leído el valor de "contrasena"
            if (clave == "contrasena" && datos.size() == 5)
            {
                std::cout << "Cargando usuario: " 
                          << datos[0] << ", " 
                          << datos[1] << ", " 
                          << datos[2] << ", " 
                          << datos[3] << ", " 
                          << datos[4] << std::endl;
                Usuario usuario(datos[0], datos[1], datos[2], datos[3], datos[4]);
                listaUsuarios.agregarUsuario(usuario);
                datos.clear(); 
            }
        }
    }

    archivo.close();
    return listaUsuarios;
}


private:
    Nodo *cabeza;
};

std::string admin_correo = "admin@gmail.com";
std::string admin_contrasena = "EDD2S2024";

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    int opcion;

    ListaUsuarios listaUsuarios;
    ListaPublicaciones listaPublicaciones;
    do
    {
        std::cout << "MENU" << std::endl;
        std::cout << "1. Iniciar sesión" << std::endl;
        std::cout << "2. Registrarse" << std::endl;
        std::cout << "3. Información" << std::endl;
        std::cout << "4. Salir" << std::endl;
        std::cout << "Ingrese su opción: ";
        std::cin >> opcion;

        // Manejo de errores de entrada
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
            opcion = 0;
        }
        else
        {
            switch (opcion)
            {
            case 1:
            {
                std::cout << "Ha seleccionado la opción 1." << std::endl;

                // Solicitar correo y contraseña
                std::string correo;
                std::string contrasena;
                std::string archivo;

                std::cout << "Ingrese su correo: ";
                std::cin >> correo;

                std::cout << "Ingrese su contraseña: ";
                std::cin >> contrasena;

                if (correo == admin_correo && contrasena == admin_contrasena)
                {
                    std::cout << "Bienvenido, admin." << std::endl;

                    int admin_opcion;
                    do
                    {
                        std::cout << "\nMenu Administrador:\n";
                        std::cout << "1. Carga de usuarios\n";
                        std::cout << "2. Carga de relaciones\n";
                        std::cout << "3. Carga de publicaciones\n";
                        std::cout << "4. Gestionar usuarios\n";
                        std::cout << "5. Reportes\n";
                        std::cout << "6. Regresar al menú principal\n";
                        std::cout << "Seleccione una opción (0 para regresar al menú principal): ";
                        std::cin >> admin_opcion;

                        if (std::cin.fail())
                        {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
                            admin_opcion = -1; // Resetea la opción para evitar una selección inválida
                        }
                        else
                        {
                            switch (admin_opcion)
                            {
                            case 1:
                                std::cout << "Opción seleccionada: Carga de usuarios.\n";
                                std::cout << "Ingrese el nombre del archivo: ";
                                std::cin >> archivo;
                                listaUsuarios = ListaUsuarios::cargarUsuariosDesdeJson("../" + archivo + ".json");

                                std::cout << "===============================\n"
                                          << std::endl;
                                std::cout << "Usuarios cargados exitosamente.\n"
                                          << std::endl;
                                std::cout << "===============================\n"
                                          << std::endl;
                                break;
                            case 2:
                                std::cout << "Opción seleccionada: Carga de relaciones.\n";
                                break;
                            case 3:
                                std::cout << "Opción seleccionada: Carga de publicaciones.\n";
                                listaPublicaciones = ListaPublicaciones::cargarPublicacionesDesdeJson("../publicaciones.json");
                                listaPublicaciones.mostrarPublicaciones();

                                std::cout << "===============================\n"
                                          << std::endl;
                                std::cout << "Publicaciones cargadas exitosamente.\n"
                                          << std::endl;
                                std::cout << "===============================\n"
                                          << std::endl;
                                break;
                            case 4:
                                std::cout << "Opción seleccionada: Gestionar usuarios.\n";
                                break;
                            case 5:
                                std::cout << "Opción seleccionada: Reportes.\n";
                                break;
                            case 6:
                            case 0:
                                std::cout << "Regresando al menú principal...\n";
                                admin_opcion = 0;
                                break;
                            default:
                                std::cout << "Opción no válida. Por favor, intente nuevamente.\n";
                                break;
                            }
                        }
                    } while (admin_opcion != 0);
                }
                else
                {
                    if (listaUsuarios.buscarUsuarioPorCorreoyContrasena(correo, contrasena))
                    {
                        std::cout << "Bienvenido, " << correo << std::endl;
                        int usuario_opcion;
                        do
                        {
                            std::cout << "\nMenu Usuario:\n";
                            std::cout << "1. Perfil\n";
                            std::cout << "2. Solicitudes\n";
                            std::cout << "3. Publicaciones\n";
                            std::cout << "4. Reportes\n";
                            std::cout << "5. Salir\n";
                            std::cout << "Seleccione una opción (0 para regresar al menú principal): ";
                            std::cin >> usuario_opcion;
                        
                            if (std::cin.fail())
                            {
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
                                usuario_opcion = -1;
                            }
                            else
                            {
                                switch (usuario_opcion)
                                {
                                case 1:
                                    std::cout << "Opción seleccionada: Perfil.\n";
                                    break;
                                case 2:
                                    std::cout << "Opción seleccionada: Solicitudes.\n";
                                    break;
                                case 3:
                                    std::cout << "Opción seleccionada: Publicaciones.\n";
                                    break;
                                case 4:
                                    std::cout << "Opción seleccionada: Reportes.\n";
                                    break;
                                case 5:
                                case 0:
                                    std::cout << "Regresando al menú principal...\n";
                                    usuario_opcion = 0;
                                    break;
                                default:
                                    std::cout << "Opción no válida. Por favor, intente nuevamente.\n";
                                    break;
                                }
                            }
                        } while (usuario_opcion != 0);
                    }
                    else
                    {
                        std::cout << "Correo o contraseña incorrectos." << std::endl;
                    }
                }
                break;
            }
            case 2:
            {
                std::cout << "Ha seleccionado la opción 2." << std::endl;

                break;
            }
            case 3:
            {
                    std::cout <<"-----------------Informacion del estudiante-----------------" << std::endl;
                    std::cout << "Nombre: Kevin Andrés Alvarez Herrera" << std::endl;
                    std::cout << "Carnet: 202203038" << std::endl;
                    std::cout << "Ingenieria en ciencias y sistemas" << std::endl;
                    std::cout << "Curso: Estructura de datos" << std::endl;
                    std::cout << "Seccion: C" << std::endl;
                    std::cout << "----------------------------------" << std::endl;
                break;
            }
            case 4:
                std::cout << "Saliendo del programa..." << std::endl;
                break;
            default:
                std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
                break;
            }
        }

        std::cout << std::endl;

    } while (opcion != 4);

    return 0;
}
