#include <iostream>
#include <limits>
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

    void borrarUsuarioPorCorreo(const std::string &correo)
    {
        if (cabeza == nullptr) // Si la lista está vacía, no hay nada que borrar
        {
            std::cerr << "La lista está vacía." << std::endl;
            return;
        }

        // Si el nodo a eliminar es el primero
        if (cabeza->usuario.getCorreo() == correo)
        {
            Nodo *temp = cabeza;
            cabeza = cabeza->siguiente; // Actualizar cabeza de la lista
            delete temp;                // Eliminar el nodo
            std::cout << "Usuario con correo " << correo << " ha sido borrado." << std::endl;
            return;
        }

        Nodo *actual = cabeza;
        Nodo *anterior = nullptr;

        // Buscar el nodo con el correo especificado
        while (actual != nullptr && actual->usuario.getCorreo() != correo)
        {
            std::cout << "Chequeando nodo con correo: " << actual->usuario.getCorreo() << std::endl;
            anterior = actual;
            actual = actual->siguiente;
        }

        // Si no se encontró el nodo
        if (actual == nullptr)
        {
            std::cerr << "No hay usuario con el correo " << correo << " que se pueda borrar." << std::endl;
            return;
        }

        // Eliminar el nodo encontrado
        if (anterior != nullptr) // Verificar que anterior no sea nulo
        {
            std::cout << "Eliminando nodo con correo: " << actual->usuario.getCorreo() << std::endl;
            anterior->siguiente = actual->siguiente;
            return;
        }

        delete actual; // Eliminar el nodo
        std::cout << "Usuario con correo " << correo << " ha sido borrado." << std::endl;

        // Verificar el estado de la lista después de la eliminación
        Nodo *temp = cabeza;
        while (temp != nullptr)
        {
            std::cout << "Nodo en lista con correo: " << temp->usuario.getCorreo() << std::endl;
            temp = temp->siguiente;
        }
    }

    void mostrarDatosPorCorreo(const std::string& correoBuscado) const
    {
        Nodo* temp = cabeza;
        bool encontrado = false;

        std::cout << "Depuración: Comenzando búsqueda del usuario con correo: " << correoBuscado << std::endl;

        while (temp != nullptr)
        {
            std::cout << "Depuración: Chequeando nodo con correo: " << temp->usuario.getCorreo() << std::endl;
            
            if (temp->usuario.getCorreo() == correoBuscado)
            {
                // Mostrar los datos del usuario encontrado
                std::cout << "Depuración: Usuario con correo " << correoBuscado << " encontrado." << std::endl;

                try
                {
                    std::cout << "Usuario encontrado:" << std::endl;
                    std::cout << "Nombre: " << temp->usuario.getNombre() << std::endl;
                    std::cout << "Apellido: " << temp->usuario.getApellido() << std::endl;
                    std::cout << "Fecha de nacimiento: " << temp->usuario.getFechaDeNacimiento() << std::endl;
                    std::cout << "Correo: " << temp->usuario.getCorreo() << std::endl;
                    std::cout << "Contraseña: " << temp->usuario.getContrasena() << std::endl;
                }
                catch (const std::bad_alloc& e)
                {
                    std::cerr << "Error de asignación de memoria: " << e.what() << std::endl;
                }
                encontrado = true;
                break;
            }
            
            temp = temp->siguiente;
        }

        if (!encontrado)
        {
            std::cerr << "Depuración: No se encontró un usuario con el correo " << correoBuscado << "." << std::endl;
            std::cerr << "No se encontró un usuario con el correo " << correoBuscado << "." << std::endl;
        }
        else
        {
            std::cout << "Depuración: Usuario con correo " << correoBuscado << " ha sido mostrado." << std::endl;
        }
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

void registrarUsuario(ListaUsuarios &listaUsuarios)
{
    std::string nombre, apellido, fecha_de_nacimiento, correo, contrasena;

    std::cout << "Ingrese su nombre: ";
    std::cin >> nombre;

    if (!nombre.empty())
    {
        std::cout << "Ingrese su apellido: ";
        std::cin >> apellido;

        if (!apellido.empty())
        {
            std::cout << "Ingrese su fecha de nacimiento (DD/MM/AAAA): ";
            std::cin >> fecha_de_nacimiento;

            if (!fecha_de_nacimiento.empty())
            {
                std::cout << "Ingrese su correo: ";
                std::cin >> correo;

                if (!correo.empty())
                {
                    if (!listaUsuarios.usuarioDuplicado(correo))
                    {
                        std::cout << "Ingrese su contraseña: ";
                        std::cin >> contrasena;
                        // Crear el nuevo usuario
                        Usuario nuevoUsuario(nombre, apellido, fecha_de_nacimiento, correo, contrasena);

                        // Agregarlo a la lista de usuarios
                        listaUsuarios.agregarUsuario(nuevoUsuario);

                        std::cout << "Usuario registrado exitosamente." << std::endl;
                    }
                    else
                    {
                        std::cerr << "El correo ya está registrado." << std::endl;
                    }
                }
                else
                {
                    std::cerr << "El correo no puede estar vacío." << std::endl;
                }
            }
            else
            {
                std::cerr << "La fecha de nacimiento no puede estar vacía." << std::endl;
            }
        }
        else
        {
            std::cerr << "El apellido no puede estar vacío." << std::endl;
        }
    }
    else
    {
        std::cerr << "El nombre no puede estar vacío." << std::endl;
    }
}

class Relacion
{
public:
    Relacion(std::string emisor, std::string receptor, std::string estado)
        : emisor_(emisor), receptor_(receptor), estado_(estado)
    {
        std::cout << "Depuración: Relación creada con emisor: " << emisor_
                  << ", receptor: " << receptor_
                  << ", estado: " << estado_ << std::endl;
    }

    std::string getEmisor() const { return emisor_; }
    void setEmisor(const std::string &emisor)
    {
        std::cout << "Depuración: Cambiando emisor de " << emisor_ << " a " << emisor << std::endl;
        emisor_ = emisor;
    }

    std::string getReceptor() const { return receptor_; }
    void setReceptor(const std::string &receptor)
    {
        std::cout << "Depuración: Cambiando receptor de " << receptor_ << " a " << receptor << std::endl;
        receptor_ = receptor;
    }

    std::string getEstado() const { return estado_; }
    void setEstado(const std::string &estado)
    {
        std::cout << "Depuración: Cambiando estado de " << estado_ << " a " << estado << std::endl;
        estado_ = estado;
    }

private:
    std::string emisor_;
    std::string receptor_;
    std::string estado_;
};

struct NodoRelacion
{
    Relacion relacion;
    NodoRelacion *siguiente;

    NodoRelacion(const Relacion &relacion) : relacion(relacion), siguiente(nullptr)
    {
        std::cout << "Depuración: Nodo de relación creado para emisor: " << relacion.getEmisor()
                  << ", receptor: " << relacion.getReceptor()
                  << ", estado: " << relacion.getEstado() << std::endl;
    }
};

class ListaRelaciones
{
public:
    ListaRelaciones() : cabeza(nullptr)
    {
        std::cout << "Depuración: Lista de relaciones creada." << std::endl;
    }

    ~ListaRelaciones()
    {
        NodoRelacion *actual = cabeza;
        while (actual != nullptr)
        {
            NodoRelacion *temp = actual;
            actual = actual->siguiente;
            std::cout << "Depuración: Eliminando nodo de relación con emisor: " << temp->relacion.getEmisor()
                      << ", receptor: " << temp->relacion.getReceptor() << std::endl;
            delete temp;
        }
    }

    void agregarRelacion(const Relacion &relacion)
    {
        std::cout << "Depuración: Agregando relación con emisor: " << relacion.getEmisor()
                  << ", receptor: " << relacion.getReceptor() << std::endl;
        NodoRelacion *nuevoNodo = new NodoRelacion(relacion);
        if (cabeza == nullptr)
        {
            cabeza = nuevoNodo;
            std::cout << "Depuración: Relación agregada como cabeza de la lista." << std::endl;
        }
        else
        {
            NodoRelacion *actual = cabeza;
            while (actual->siguiente != nullptr)
            {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
            std::cout << "Depuración: Relación agregada al final de la lista." << std::endl;
        }
    }

    static ListaRelaciones cargarRelacionesDesdeJson(const std::string &nombreArchivo)
    {
        ListaRelaciones listaRelaciones;
        std::ifstream archivo(nombreArchivo);

        if (!archivo.is_open())
        {
            std::cerr << "Error al abrir el archivo JSON." << std::endl;
            return listaRelaciones;
        }

        std::string linea;
        std::string emisor, receptor, estado;

        std::cout << "Depuración: Comenzando a cargar relaciones desde " << nombreArchivo << std::endl;

        while (std::getline(archivo, linea))
        {
            linea.erase(std::remove(linea.begin(), linea.end(), ' '), linea.end());
            linea.erase(std::remove(linea.begin(), linea.end(), '\"'), linea.end());

            if (linea.find("emisor:") != std::string::npos)
            {
                emisor = linea.substr(linea.find(":") + 1);
                if (!emisor.empty() && emisor.back() == ',')
                {
                    emisor.pop_back(); // Elimina la coma final si existe
                }
                std::cout << "Depuración: Emisor encontrado: " << emisor << std::endl;
            }
            else if (linea.find("receptor:") != std::string::npos)
            {
                receptor = linea.substr(linea.find(":") + 1);
                if (!receptor.empty() && receptor.back() == ',')
                {
                    receptor.pop_back(); // Elimina la coma final si existe
                }
                std::cout << "Depuración: Receptor encontrado: " << receptor << std::endl;
            }
            else if (linea.find("estado:") != std::string::npos)
            {
                estado = linea.substr(linea.find(":") + 1);
                if (!estado.empty() && estado.back() == ',')
                {
                    estado.pop_back(); // Elimina la coma final si existe
                }
                std::cout << "Depuración: Estado encontrado: " << estado << std::endl;
                Relacion relacion(emisor, receptor, estado);
                listaRelaciones.agregarRelacion(relacion);
            }
        }

        archivo.close();
        std::cout << "Depuración: Finalizada la carga de relaciones desde " << nombreArchivo << std::endl;
        return listaRelaciones;
    }

private:
    NodoRelacion *cabeza;
};

class Publicacion
{
public:
    Publicacion(std::string correo, std::string contenido, std::string fecha, std::string hora)
        : correo_(correo), contenido_(contenido), fecha_(fecha), hora_(hora)
    {
        std::cout << "Depuración: Publicación creada con correo: " << correo_
                  << ", contenido: " << contenido_
                  << ", fecha: " << fecha_
                  << ", hora: " << hora_ << std::endl;
    }

    std::string getCorreo() const { return correo_; }
    void setCorreo(const std::string &correo)
    {
        std::cout << "Depuración: Cambiando correo de " << correo_ << " a " << correo << std::endl;
        correo_ = correo;
    }

    std::string getContenido() const { return contenido_; }
    void setContenido(const std::string &contenido)
    {
        std::cout << "Depuración: Cambiando contenido de \"" << contenido_ << "\" a \"" << contenido << "\"" << std::endl;
        contenido_ = contenido;
    }

    std::string getFecha() const { return fecha_; }
    void setFecha(const std::string &fecha)
    {
        std::cout << "Depuración: Cambiando fecha de " << fecha_ << " a " << fecha << std::endl;
        fecha_ = fecha;
    }

    std::string getHora() const { return hora_; }
    void setHora(const std::string &hora)
    {
        std::cout << "Depuración: Cambiando hora de " << hora_ << " a " << hora << std::endl;
        hora_ = hora;
    }

private:
    std::string correo_;
    std::string contenido_;
    std::string fecha_;
    std::string hora_;

    friend class ListaPublicaciones;
};

struct NodoPublicacion
{
    Publicacion publicacion;
    NodoPublicacion *siguiente;

    NodoPublicacion(const Publicacion &publicacion) : publicacion(publicacion), siguiente(nullptr)
    {
        std::cout << "Depuración: Nodo de publicación creado para correo: " << publicacion.getCorreo() << std::endl;
    }
};

// Lista simplemente enlazada para Publicaciones
class ListaPublicaciones
{
public:
    ListaPublicaciones() : cabeza(nullptr)
    {
        std::cout << "Depuración: Lista de publicaciones creada." << std::endl;
    }

    ~ListaPublicaciones()
    {
        NodoPublicacion *actual = cabeza;
        while (actual != nullptr)
        {
            NodoPublicacion *temp = actual;
            actual = actual->siguiente;
            std::cout << "Depuración: Eliminando nodo de publicación con correo: " << temp->publicacion.getCorreo() << std::endl;
            delete temp;
        }
    }

    void agregarPublicacion(const Publicacion &publicacion)
    {
        std::cout << "Depuración: Agregando publicación con correo: " << publicacion.getCorreo() << std::endl;
        NodoPublicacion *nuevoNodo = new NodoPublicacion(publicacion);
        if (cabeza == nullptr)
        {
            cabeza = nuevoNodo;
            std::cout << "Depuración: Publicación agregada como cabeza de la lista." << std::endl;
        }
        else
        {
            NodoPublicacion *actual = cabeza;
            while (actual->siguiente != nullptr)
            {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
            std::cout << "Depuración: Publicación agregada al final de la lista." << std::endl;
        }
    }

    static ListaPublicaciones cargarPublicacionesDesdeJson(const std::string &nombreArchivo)
    {
        ListaPublicaciones listaPublicaciones;
        std::ifstream archivo(nombreArchivo);

        if (!archivo.is_open())
        {
            std::cerr << "Error al abrir el archivo JSON." << std::endl;
            return listaPublicaciones;
        }

        std::string linea;
        std::string correo, contenido, fecha, hora;

        std::cout << "Depuración: Comenzando a cargar publicaciones desde " << nombreArchivo << std::endl;

        while (std::getline(archivo, linea))
        {
            linea.erase(std::remove(linea.begin(), linea.end(), ' '), linea.end());
            linea.erase(std::remove(linea.begin(), linea.end(), '\"'), linea.end());

            if (linea.find("correo:") != std::string::npos)
            {
                correo = linea.substr(linea.find(":") + 1);
                if (!correo.empty() && correo.back() == ',')
                {
                    correo.pop_back(); // Elimina la coma final si existe
                }
                std::cout << "Depuración: Correo encontrado: " << correo << std::endl;
            }
            else if (linea.find("contenido:") != std::string::npos)
            {
                contenido = linea.substr(linea.find(":") + 1);
                if (!contenido.empty() && contenido.back() == ',')
                {
                    contenido.pop_back(); // Elimina la coma final si existe
                }
                std::cout << "Depuración: Contenido encontrado: " << contenido << std::endl;
            }
            else if (linea.find("fecha:") != std::string::npos)
            {
                fecha = linea.substr(linea.find(":") + 1);
                if (!fecha.empty() && fecha.back() == ',')
                {
                    fecha.pop_back(); // Elimina la coma final si existe
                }
                std::cout << "Depuración: Fecha encontrada: " << fecha << std::endl;
            }
            else if (linea.find("hora:") != std::string::npos)
            {
                hora = linea.substr(linea.find(":") + 1);
                if (!hora.empty() && hora.back() == ',')
                {
                    hora.pop_back(); // Elimina la coma final si existe
                }
                std::cout << "Depuración: Hora encontrada: " << hora << std::endl;
                Publicacion publicacion(correo, contenido, fecha, hora);
                listaPublicaciones.agregarPublicacion(publicacion);
            }
        }

        archivo.close();
        std::cout << "Depuración: Finalizada la carga de publicaciones desde " << nombreArchivo << std::endl;
        return listaPublicaciones;
    }

private:
    NodoPublicacion *cabeza;
};

std::string admin_correo = "admin@gmail.com";
std::string admin_contrasena = "EDD2S2024";

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    int opcion;

    ListaUsuarios listaUsuarios;
    ListaPublicaciones listaPublicaciones;
    ListaRelaciones listaRelaciones;
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
                std::string archivo_P;
                std::string archivo_R;
                std::string correo_e;

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
                                std::cout << "Ingrese el nombre del archivo: ";
                                std::cin >> archivo_R;
                                listaRelaciones = ListaRelaciones::cargarRelacionesDesdeJson("../" + archivo_R + ".json");

                                std::cout << "===============================\n"
                                          << std::endl;
                                std::cout << "Relaciones cargadas exitosamente.\n"
                                          << std::endl;
                                std::cout << "===============================\n"
                                          << std::endl;
                                break;
                            case 3:
                                std::cout << "Opción seleccionada: Carga de publicaciones.\n";
                                std::cout << "Ingrese el nombre del archivo: ";
                                std::cin >> archivo_P;
                                listaPublicaciones = ListaPublicaciones::cargarPublicacionesDesdeJson("../" + archivo_P + ".json");

                                std::cout << "===============================\n"
                                          << std::endl;
                                std::cout << "Publicaciones cargadas exitosamente.\n"
                                          << std::endl;
                                std::cout << "===============================\n"
                                          << std::endl;
                                break;
                            case 4:
                                std::cout << "Opción seleccionada: Gestionar usuarios.\n";
                                std::cout << "Ingrese el correo del usuario a borrar: ";
                                std::cin >> correo_e;

                                if (correo_e.empty())
                                {
                                    std::cerr << "No se ingresó un correo válido." << std::endl;
                                }
                                else
                                {
                                    listaUsuarios.borrarUsuarioPorCorreo(correo_e);
                                }
                                break;
                            case 5:
                                int reportes_opcion;
                                std::cout << "Opción seleccionada: Reportes.\n";

                                do
                                {
                                    std::cout << "-----------------Reportes-----------------" << std::endl;
                                    std::cout << "1. Usuarios " << std::endl;
                                    std::cout << "2. Relaciones de Amistad" << std::endl;
                                    std::cout << "3. Publicaciones" << std::endl;
                                    std::cout << "4. Top" << std::endl;
                                    std::cout << "5. Regresar al menú principal" << std::endl;
                                    std::cout << "Seleccione una opción (0 para regresar al menú principal): ";
                                    std::cin >> reportes_opcion;
                                    switch (reportes_opcion)
                                    {
                                    case 1:
                                        std::cout << "Opción seleccionada: Reporte de Usuarios.\n";

                                        break;
                                    case 2:
                                        std::cout << "Opción seleccionada: Reporte de Relaciones de Amistad.\n";

                                        break;
                                    case 3:
                                        std::cout << "Opción seleccionada: Reporte de Publicaciones.\n";

                                        break;
                                    case 4:
                                        std::cout << "Opción seleccionada: Reporte de Top.\n";

                                        break;
                                    case 5:
                                    case 0:
                                        std::cout << "Regresando al menú principal...\n";
                                        reportes_opcion = 0;
                                        break;
                                    default:
                                        break;
                                    }
                                } while (reportes_opcion != 0);

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
                                int perfil_opcion;
                                std::cout << "1. Ver perfil\n";
                                std::cout << "2. Eliminar perfil\n";
                                std::cout << "Seleccione una opción (0 para regresar al menú principal): ";
                                std::cin >> perfil_opcion;
                                if (perfil_opcion == 1)
                                {
                                    listaUsuarios.mostrarDatosPorCorreo(correo);
                                    break;
                                }else if (perfil_opcion == 2)
                                {
                                    listaUsuarios.borrarUsuarioPorCorreo(correo);
                                    usuario_opcion = 0;
                                    break;
                                }
                                else
                                {
                                    std::cout << "Opción no válida. Por favor, intente nuevamente.\n";
                                }
                                    break;
                                case 2:
                                    std::cout << "Opción seleccionada: Solicitudes.\n";
                                    break;
                                case 3:
                                    std::cout << "Opción seleccionada: Publicaciones.\n";
                                    break;
                                case 4:
                                int reportes_opcion_usuario;
                                    std::cout << "Opción seleccionada: Reportes.\n";
                                    do
                                    {
                                        std::cout << "-----------------Reportes-----------------" << std::endl;
                                        std::cout << "1. Solicitudes Enviadas y Recibidas " << std::endl;
                                        std::cout << "2. Relaciones de Amistad" << std::endl;
                                        std::cout << "3. Publicaciones" << std::endl;
                                        std::cout << "4. Mis Amigos" << std::endl;
                                        std::cout << "5. Regresar al menú principal" << std::endl;
                                        std::cout << "Seleccione una opción (0 para regresar al menú principal): ";
                                        std::cin >> reportes_opcion_usuario;
                                        switch (reportes_opcion_usuario)
                                        {
                                        case 1:
                                            std::cout << "Opción seleccionada: Solicitudes Enviadas y Recibidas.\n";

                                            break;
                                        case 2:
                                            std::cout << "Opción seleccionada: Reporte de Relaciones de Amistad.\n";

                                            break;
                                        case 3:
                                            std::cout << "Opción seleccionada: Reporte de Publicaciones.\n";

                                            break;
                                        case 4:
                                            std::cout << "Opción seleccionada: Mis Amigos.\n";

                                            break;
                                        case 5:
                                        case 0:
                                            std::cout << "Regresando al menú principal...\n";
                                            reportes_opcion_usuario = 0;
                                            break;
                                        default:
                                            break;
                                        }
                                    } while (reportes_opcion_usuario != 0);

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
                std::cout << "Ha seleccionado la opción REGISTRARSE." << std::endl;
                registrarUsuario(listaUsuarios);
                break;
            }

            case 3:
            {
                std::cout << "-----------------Informacion del estudiante-----------------" << std::endl;
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
