#include <iostream>
#include <limits>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include "json.hpp"
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
    friend class ListaUsuarios;

public:
    std::string nombre_;
    std::string apellido_;
    std::string fecha_de_nacimiento_;
    std::string correo_;
    std::string contrasena_;

    friend class ListaUsuarios;
};

class Nodo
{
public:
    Usuario usuario;
    Nodo *siguiente;

    Nodo(const Usuario &usuario) : usuario(usuario), siguiente(nullptr)
    {
        std::cout << "Depuración: Nodo creado para usuario con correo: " << usuario.getCorreo() << std::endl;
    }
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
            temp = nullptr;
        }
    }

    void generateDot(const std::string &filename) const
    {
        std::ofstream file(filename);
        if (file.is_open())
        {
            file << "digraph G {" << std::endl;
            file << "node [shape=record];" << std::endl;
            file << "rankdir=LR;" << std::endl;

            Nodo *current = cabeza;
            int id = 0;
            while (current != nullptr)
            {
                file << "node" << id << " [label=\"{" << "Nombre: " << current->usuario.getNombre() << "\\n"
                     << "Correo: " << current->usuario.getCorreo() << "}\"];" << std::endl;
                if (current->siguiente != nullptr)
                {
                    file << "node" << id << " -> node" << (id + 1) << ";" << std::endl;
                }
                current = current->siguiente;
                id++;
            }

            file << "}" << std::endl;
            file.close();
            delete current;
            current = nullptr;
        }
        else
        {
            std::cerr << "No se pudo abrir el archivo" << std::endl;
        }
    }

    void renderGraphviz(const std::string &dotFilename, const std::string &imageFilename) const
    {
        std::string command = "dot -Tpng " + dotFilename + " -o " + imageFilename;
        int result = system(command.c_str());
        if (result != 0)
        {
            std::cerr << "Error al generar la imagen con Graphviz" << std::endl;
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

        Nodo *nuevoNodo = nullptr;

        try
        {
            nuevoNodo = new Nodo(usuario);
        }
        catch (const std::bad_alloc &e)
        {
            std::cerr << "Error de asignación de memoria al agregar usuario: " << e.what() << std::endl;
            return;
        }

        if (cabeza == nullptr)
        {
            // Si la lista está vacía, el nuevo nodo es tanto la cabeza como la cola
            cabeza = nuevoNodo;
            cola = nuevoNodo;
            std::cout << "Usuario agregado como cabeza de la lista." << std::endl;
        }
        else
        {
            // Agregar el nuevo nodo después de la cola actual
            cola->siguiente = nuevoNodo;
            cola = nuevoNodo; // Actualizar la cola para que apunte al nuevo nodo
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
        actual = nullptr;
        delete actual;
        return false;
    };

    bool buscarUsuarioPorCorreoyContrasena(const std::string &correo, const std::string &contrasena) const
    {
        Nodo *temp = cabeza;
        while (temp != nullptr)
        {
            if (temp->usuario.getCorreo() == correo && temp->usuario.getContrasena() == contrasena)
            {
                temp = nullptr;
                delete temp;
                return true;
            }
            temp = temp->siguiente;
        }
        return false;
    };

    bool buscarCorreo(const std::string &correo) const
    {
        Nodo *temp = cabeza;
        while (temp != nullptr)
        {
            if (temp->usuario.getCorreo() == correo)
            {
                temp = nullptr;
                delete temp;
                return true;
            }
            temp = temp->siguiente;
        }
        return false;
    };

    void borrarUsuarioPorCorreo(const std::string &correo)
    {
        if (cabeza == nullptr)
        {
            std::cerr << "La lista está vacía." << std::endl;
            return;
        }

        if (cabeza->usuario.getCorreo() == correo)
        {
            std::cout << "Eliminando nodo con correo: " << correo << std::endl;
            Nodo *temp = cabeza;
            cabeza = cabeza->siguiente;

            if (cabeza == nullptr)
            {
                std::cout << "Lista está ahora vacía después de eliminar el nodo." << std::endl;
            }
            else
            {
                std::cout << "Nueva cabeza después de eliminar nodo: " << cabeza->usuario.getCorreo() << std::endl;
            }

            temp = nullptr;
            delete temp;
            return;
        }

        Nodo *actual = cabeza;
        Nodo *anterior = nullptr;

        while (actual != nullptr && actual->usuario.getCorreo() != correo)
        {
            std::cout << "Chequeando nodo con correo: " << actual->usuario.getCorreo() << std::endl;
            anterior = actual;
            actual = actual->siguiente;
        }

        if (actual == nullptr)
        {
            std::cerr << "No hay usuario con el correo " << correo << " que se pueda borrar." << std::endl;
            return;
        }

        if (anterior != nullptr)
        {
            std::cout << "Eliminando nodo con correo: " << actual->usuario.getCorreo() << std::endl;
            anterior->siguiente = actual->siguiente;
            return;
        }

        actual->siguiente = nullptr;

        delete actual;
        actual = nullptr;
        std::cout << "Usuario con correo " << correo << " ha sido borrado." << std::endl;

        return;
    }

    void mostrarDatosPorCorreo(const std::string &correo) const
    {
        Nodo *cabeza = this->cabeza;
        Nodo *temp = cabeza;
        bool encontrado = false;

        while (temp != nullptr)
        {
            if (temp->usuario.getCorreo() == correo)
            {
                std::cout << "Usuario encontrado:" << std::endl;
                std::cout << "Nombre: " << temp->usuario.getNombre() << std::endl;
                std::cout << "Apellido: " << temp->usuario.getApellido() << std::endl;
                std::cout << "Fecha de nacimiento: " << temp->usuario.getFechaDeNacimiento() << std::endl;
                std::cout << "Correo: " << temp->usuario.getCorreo() << std::endl;
                std::cout << "Contraseña: " << temp->usuario.getContrasena() << std::endl;
                encontrado = true;
                break;
            }
            temp = temp->siguiente;
        }

        if (!encontrado)
        {
            std::cerr << "No se encontró un usuario con el correo " << correo << "." << std::endl;
        }
    }

    std::string limpiarCadena(const std::string &str)
    {
        std::string resultado = str;
        resultado.erase(0, resultado.find_first_not_of(" \n\r\t\""));
        resultado.erase(resultado.find_last_not_of(" \n\r\t\"") + 1);
        return resultado;
    }

    void cargarUsuariosDesdeJson(const std::string &nombreArchivo)
    {
        std::ifstream archivo(nombreArchivo);

        if (!archivo.is_open())
        {
            std::cerr << "Error al abrir el archivo JSON." << std::endl;
            return;
        }

        try
        {
            nlohmann::json jsonData;
            archivo >> jsonData;

            for (const auto &item : jsonData)
            {
                std::string nombre = limpiarCadena(item.at("nombre").get<std::string>());
                std::string apellido = limpiarCadena(item.at("apellido").get<std::string>());
                std::string fecha_de_nacimiento = limpiarCadena(item.at("fecha_de_nacimiento").get<std::string>());
                std::string correo = limpiarCadena(item.at("correo").get<std::string>());
                std::string contrasena = limpiarCadena(item.at("contrasena").get<std::string>());

                Usuario usuario(nombre, apellido, fecha_de_nacimiento, correo, contrasena);
                agregarUsuario(usuario);
            }
        }
        catch (const nlohmann::json::exception &e)
        {
            std::cerr << "Error al procesar el archivo JSON: " << e.what() << std::endl;
        }
    }

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

private:
    Nodo *cabeza;
    Nodo *cola;
};

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

class NodoRelacion
{
public:
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
    NodoRelacion *obtenerCabeza() const { return cabeza; }

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
            temp = nullptr;
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

    void cargarRelacionesDesdeJson(const std::string &nombreArchivo)
    {
        std::ifstream archivo(nombreArchivo);
        if (archivo.is_open())
        {
            nlohmann::json jsonData;
            archivo >> jsonData;
            archivo.close();

            for (const auto &item : jsonData)
            {
                std::string emisor = item["emisor"];
                std::string receptor = item["receptor"];
                std::string estado = item["estado"];

                Relacion nuevaRelacion(emisor, receptor, estado);
                agregarRelacion(nuevaRelacion);
            }

            std::cout << "Depuración: Finalizada la carga de relaciones desde " << nombreArchivo << std::endl;
        }
        else
        {
            std::cerr << "Error al abrir el archivo JSON." << std::endl;
        }
        std::cout << "===============================\n";
        std::cout << "Relaciones cargadas exitosamente.\n";
        std::cout << "===============================\n";
    }

private:
    NodoRelacion *cabeza;
};

class Emisor
{
public:
    Emisor(std::string correo, std::string receptor, std::string estado)
        : correo(correo), receptor(receptor), estado(estado)
    {
        std::cout << "Depuración: Emisor creado con correo: " << correo
                  << ", receptor: " << receptor
                  << ", estado: " << estado << std::endl;
    }
    std::string getCorreo() const { return correo; }
    std::string getReceptor() const { return receptor; }
    std::string getEstado() const { return estado; }
    void setCorreo(const std::string &nuevoCorreo)
    {
        std::cout << "Depuración: Cambiando correo de " << correo << " a " << nuevoCorreo << std::endl;
        correo = nuevoCorreo;
    }
    void setReceptor(const std::string &nuevoReceptor)
    {
        std::cout << "Depuración: Cambiando receptor de " << receptor << " a " << nuevoReceptor << std::endl;
        receptor = nuevoReceptor;
    }
    void setEstado(const std::string &nuevoEstado)
    {
        std::cout << "Depuración: Cambiando estado de " << estado << " a " << nuevoEstado << std::endl;
        estado = nuevoEstado;
    }

private:
    std::string correo;
    std::string receptor;
    std::string estado;
};

class Nodo_emisor
{
public:
    Emisor emisor;
    Nodo_emisor *siguiente;

    Nodo_emisor(const Emisor &emisor) : emisor(emisor), siguiente(nullptr) {}
};

class ListaEmisor
{
public:
    ListaEmisor() : cabeza(nullptr) {}
    Nodo_emisor *obtenerCabeza() const { return cabeza; }

    ~ListaEmisor()
    {
        while (cabeza)
        {
            Nodo_emisor *temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }

    void agregarEmisor(const Emisor &emisor)
    {
        Nodo_emisor *nuevoNodo = new Nodo_emisor(emisor);
        nuevoNodo->siguiente = cabeza;
        cabeza = nuevoNodo;

        std::cout << "Depuración: Emisor agregado con correo: " << emisor.getCorreo() << std::endl;
    }

    void mostrarEmisores(const ListaRelaciones &listaRelaciones, const std::string &correo) const
    {
        if (cabeza == nullptr)
        {
            // Si la lista de emisores está vacía, buscar en la lista de relaciones
            bool encontrado = false;
            NodoRelacion *actualRelacion = listaRelaciones.obtenerCabeza();
            while (actualRelacion)
            {
                if (actualRelacion->relacion.getEmisor() == correo)
                {
                    encontrado = true;
                    std::cout << "========================================" << std::endl;
                    std::cout << "\n----------------Enviadas----------------";
                    std::cout << "\n========================================" << std::endl;
                    std::cout << "Correo: " << actualRelacion->relacion.getEmisor()
                            << ", Receptor: " << actualRelacion->relacion.getReceptor()
                            << ", Estado: " << actualRelacion->relacion.getEstado()
                            << "\n========================================"
                            << std::endl;
                    std::cout << "La solicitud está en la lista de relaciones." << std::endl;
                    std::cout << "Estado en la lista de relaciones: " << actualRelacion->relacion.getEstado() << std::endl;
                }
                actualRelacion = actualRelacion->siguiente;
            }

            if (!encontrado)
            {
                std::cout << "No hay solicitudes de amistad para el correo proporcionado en la lista de relaciones." << std::endl;
            }
        }
        else
        {
            // Si la lista de emisores no está vacía, mostrar y buscar en ambas listas
            Nodo_emisor *actual = cabeza;
            bool encontradoEnEmisores = false;

            while (actual)
            {
                if (actual->emisor.getCorreo() == correo)
                {
                    encontradoEnEmisores = true;
                    std::cout << "========================================" << std::endl;
                    std::cout << "\n----------------Enviadas----------------";
                    std::cout << "\n========================================" << std::endl;
                    std::cout << "Correo: " << actual->emisor.getCorreo()
                            << ", Receptor: " << actual->emisor.getReceptor()
                            << ", Estado: " << actual->emisor.getEstado()
                            << "\n========================================"
                            << std::endl;
                }
                actual = actual->siguiente;
            }

            if (encontradoEnEmisores)
            {
                // Verificar en la lista de relaciones
                bool encontradoEnRelaciones = false;
                NodoRelacion *actualRelacion = listaRelaciones.obtenerCabeza();
                while (actualRelacion)
                {
                    if (actualRelacion->relacion.getEmisor() == correo)
                    {
                        encontradoEnRelaciones = true;
                        std::cout << "========================================" << std::endl;
                        std::cout << "\n----------------Enviadas----------------";
                        std::cout << "\n========================================" << std::endl;
                        std::cout << "Correo: " << actualRelacion->relacion.getEmisor()
                                << ", Receptor: " << actualRelacion->relacion.getReceptor()
                                << ", Estado: " << actualRelacion->relacion.getEstado()
                                << "\n========================================"
                                << std::endl;
                        std::cout << "La solicitud está en la lista de relaciones." << std::endl;
                        std::cout << "Estado en la lista de relaciones: " << actualRelacion->relacion.getEstado() << std::endl;
                    }
                    actualRelacion = actualRelacion->siguiente;
                }

                if (!encontradoEnRelaciones)
                {
                    std::cout << "La solicitud no está en la lista de relaciones." << std::endl;
                }
            }
            else
            {
                bool encontrado = false;
                NodoRelacion *actualRelacion = listaRelaciones.obtenerCabeza();
                while (actualRelacion)
                {
                    if (actualRelacion->relacion.getEmisor() == correo)
                    {
                        encontrado = true;
                        std::cout << "========================================" << std::endl;
                        std::cout << "\n----------------Enviadas----------------";
                        std::cout << "\n========================================" << std::endl;
                        std::cout << "Correo: " << actualRelacion->relacion.getEmisor()
                                << ", Receptor: " << actualRelacion->relacion.getReceptor()
                                << ", Estado: " << actualRelacion->relacion.getEstado()
                                << "\n========================================"
                                << std::endl;
                        std::cout << "La solicitud está en la lista de relaciones." << std::endl;
                        std::cout << "Estado en la lista de relaciones: " << actualRelacion->relacion.getEstado() << std::endl;
                    }
                    actualRelacion = actualRelacion->siguiente;
                }

                if (!encontrado)
                {
                    std::cout << "No hay solicitudes de amistad para el correo proporcionado en la lista de relaciones." << std::endl;
                }            
            }
        }
    }


    void eliminarPrimero()
    {
        if (cabeza)
        {
            Nodo_emisor *temp = cabeza;
            cabeza = cabeza->siguiente;
            std::cout << "Depuración: Emisor con correo " << temp->emisor.getCorreo() << " eliminado." << std::endl;
            delete temp;
        }
        else
        {
            std::cout << "Depuración: La lista está vacía, no se puede eliminar." << std::endl;
        }
    }

private:
    Nodo_emisor *cabeza;
};

class Receptor
{
public:
    Receptor(std::string correo, std::string emisor, std::string estado)
        : correo(correo), emisor(emisor), estado(estado)
    {
        std::cout << "Depuración: Receptor creado con correo: " << correo
                  << ", emisor: " << emisor
                  << ", estado: " << estado << std::endl;
    }
    std::string getCorreo() const { return correo; }
    std::string getEmisor() const { return emisor; }
    std::string getEstado() const { return estado; }
    void setCorreo(const std::string &nuevoCorreo)
    {
        std::cout << "Depuración: Cambiando correo de " << correo << " a " << nuevoCorreo << std::endl;
        correo = nuevoCorreo;
    }
    void setEmisor(const std::string &nuevoEmisor)
    {
        std::cout << "Depuración: Cambiando emisor de " << emisor << " a " << nuevoEmisor << std::endl;
        emisor = nuevoEmisor;
    }
    void setEstado(const std::string &nuevoEstado)
    {
        std::cout << "Depuración: Cambiando estado de " << estado << " a " << nuevoEstado << std::endl;
        estado = nuevoEstado;
    }

private:
    std::string correo;
    std::string emisor;
    std::string estado;
};

// Definición del nodo para la pila de receptores
class Nodo_PilaReceptor
{
public:
    Receptor receptor;            // Contenido del nodo
    Nodo_PilaReceptor *siguiente; // Puntero al siguiente nodo

    Nodo_PilaReceptor(const Receptor &receptor) : receptor(receptor), siguiente(nullptr) {}
};

// Definición de la pila de receptores
class PilaReceptor
{
public:
    PilaReceptor() : cima(nullptr) {}

    ~PilaReceptor()
    {
        while (cima)
        {
            Nodo_PilaReceptor *temp = cima;
            cima = cima->siguiente;
            delete temp;
            limpiarPila();
        }
    }
    void limpiarPila()
    {
        while (!estaVacia())
        {
            desapilar(); // Usa el método desapilar para vaciar la pila
        }
        std::cout << "Depuración: La pila ha sido limpiada." << std::endl;
    }
    void apilar(const Receptor &receptor)
    {
        Nodo_PilaReceptor *nuevoNodo = new Nodo_PilaReceptor(receptor);
        nuevoNodo->siguiente = cima;
        cima = nuevoNodo;

        std::cout << "Depuración: Receptor apilado con correo: " << receptor.getCorreo() << std::endl;
    }

    void buscarYApilarPendientes(const std::string &correo, const ListaEmisor &listaEmisor, const ListaRelaciones &listaRelaciones)
    {
        // Buscar en ListaEmisor
        Nodo_emisor *actualEmisor = listaEmisor.obtenerCabeza();

        while (actualEmisor)
        {
            if (actualEmisor->emisor.getReceptor() == correo && actualEmisor->emisor.getEstado() == "PENDIENTE")
            {
                apilar(Receptor(correo, actualEmisor->emisor.getCorreo(), "PENDIENTE"));
            }
            actualEmisor = actualEmisor->siguiente;
        }

        // Buscar en ListaRelaciones
        NodoRelacion *actualRelacion = listaRelaciones.obtenerCabeza();

        while (actualRelacion)
        {
            if (actualRelacion->relacion.getReceptor() == correo && actualRelacion->relacion.getEstado() == "PENDIENTE")
            {
                apilar(Receptor(correo, actualRelacion->relacion.getEmisor(), "PENDIENTE"));
            }
            actualRelacion = actualRelacion->siguiente;
        }

        if (estaVacia())
        {
            std::cout << "No se encontraron solicitudes pendientes para el correo: " << correo << std::endl;
        }
        else
        {
            std::cout << "Solicitudes pendientes apiladas para el correo: " << correo << std::endl;
        }
    }

    void desapilar()
    {
        if (cima)
        {
            Nodo_PilaReceptor *temp = cima;
            cima = cima->siguiente;
            std::cout << "Depuración: Receptor con correo " << temp->receptor.getCorreo() << " desapilado." << std::endl;
            delete temp;
        }
        else
        {
            std::cout << "Depuración: La pila está vacía, no se puede desapilar." << std::endl;
        }
    }

    void mostrarReceptores() const
    {
        Nodo_PilaReceptor *actual = cima;
        while (actual)
        {
            std::cout << "========================================" << std::endl;
            std::cout << "---------------Recibidas---------------";
            std::cout << "\n========================================" << std::endl;
            ;

            std::cout << "Correo: " << actual->receptor.getCorreo()
                      << ", Emisor: " << actual->receptor.getEmisor()
                      << ", Estado: " << actual->receptor.getEstado()
                      << "\n========================================"
                      << std::endl;

            actual = actual->siguiente;
        }
    }

    bool estaVacia() const
    {
        return cima == nullptr;
    }

private:
    Nodo_PilaReceptor *cima;

    bool esDuplicado(const Receptor &receptor) const
    {
        Nodo_PilaReceptor *actual = cima;
        while (actual)
        {
            if (actual->receptor.getCorreo() == receptor.getCorreo())
            {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }
};

class Publicacion
{
public:
    Publicacion(int id, std::string correo, std::string contenido, std::string fecha, std::string hora)
        : id_(id), correo_(correo), contenido_(contenido), fecha_(fecha), hora_(hora)
    {
        std::cout << "Depuración: Publicación creada con ID: " << id_
                  << ", correo: " << correo_
                  << ", contenido: " << contenido_
                  << ", fecha: " << fecha_
                  << ", hora: " << hora_ << std::endl;
    }

    int getId() const { return id_; }
    void setID(int id)
    {
        std::cout << "Depuración: Cambiando ID de " << id_ << " a " << id << std::endl;
        id_ = id;
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
    int id_;
    std::string correo_;
    std::string contenido_;
    std::string fecha_;
    std::string hora_;

    friend class ListaPublicaciones;
};

class NodoPublicacion
{
public:
    Publicacion publicacion;
    NodoPublicacion *siguiente;
    NodoPublicacion *anterior;

    NodoPublicacion(const Publicacion &publicacion)
        : publicacion(publicacion), siguiente(nullptr), anterior(nullptr)
    {
        std::cout << "Depuración: Nodo de publicación creado para correo: " << publicacion.getCorreo() << std::endl;
    }
};

class ListaPublicaciones
{
public:
    ListaPublicaciones() : cabeza(nullptr), cola(nullptr), siguienteId(1)
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

    void generateDot(const std::string &filename) const
    {
        std::ofstream file(filename);
        if (file.is_open())
        {
            file << "digraph G {" << std::endl;
            file << "node [shape=record];" << std::endl;
            file << "rankdir=LR;" << std::endl;

            NodoPublicacion *current = cabeza;
            int id = 0;
            while (current != nullptr)
            {
                // Nodo actual
                file << "node" << id << " [label=\"{" << "Correo: " << current->publicacion.getCorreo() << "\\n"
                     << "Contenido: " << current->publicacion.getContenido() << "\\n"
                     << "Fecha: " << current->publicacion.getFecha() << "\\n"
                     << "Hora: " << current->publicacion.getHora() << "}\"];" << std::endl;

                // Conexión con el siguiente nodo
                if (current->siguiente != nullptr)
                {
                    file << "node" << id << " -> node" << (id + 1) << ";" << std::endl;
                }

                if (current->anterior != nullptr)
                {
                    int anteriorId = id - 1;
                    file << "node" << id << " -> node" << anteriorId << " [style=dashed];" << std::endl;
                }

                current = current->siguiente;
                id++;
            }

            file << "}" << std::endl;
            file.close();
        }
        else
        {
            std::cerr << "No se pudo abrir el archivo para escribir." << std::endl;
        }
    }

    void renderGraphviz(const std::string &dotFilename, const std::string &imageFilename) const
    {
        std::string command = "dot -Tpng " + dotFilename + " -o " + imageFilename;
        int result = system(command.c_str());
        if (result != 0)
        {
            std::cerr << "Error al generar la imagen con Graphviz" << std::endl;
        }
    }

    void crearPublicacion(const std::string &correo, const std::string &contenido, const std::string &fecha, const std::string &hora)
    {
        if (correo.empty())
        {
            std::cerr << "Error: No se ha establecido un correo para la publicación." << std::endl;
            return;
        }

        Publicacion nuevaPublicacion(siguienteId++, correo, contenido, fecha, hora);
        agregarPublicacion(nuevaPublicacion);
    }

    void agregarPublicacion(const Publicacion &publicacion)
    {
        std::cout << "Agregando publicación del usuario con correo: " << publicacion.getCorreo() << std::endl;

        NodoPublicacion *nuevoNodo = new NodoPublicacion(publicacion);

        if (cabeza == nullptr)
        {
            cabeza = nuevoNodo;
            cola = nuevoNodo;
            std::cout << "Publicación agregada como cabeza y cola de la lista." << std::endl;
        }
        else
        {
            cola->siguiente = nuevoNodo;
            nuevoNodo->anterior = cola;
            cola = nuevoNodo;
            std::cout << "Publicación agregada al final de la lista." << std::endl;
        }
    }

    void borrarPublicacionPorId(int id)
    {
        if (cabeza == nullptr)
        {
            std::cerr << "La lista está vacía." << std::endl;
            return;
        }

        NodoPublicacion *actual = cabeza;

        while (actual != nullptr && actual->publicacion.getId() != id)
        {
            actual = actual->siguiente;
        }

        if (actual == nullptr)
        {
            std::cerr << "No se encontró publicación con ID: " << id << "." << std::endl;
            return;
        }

        if (actual == cabeza)
        {
            cabeza = actual->siguiente;
            if (cabeza != nullptr)
            {
                cabeza->anterior = nullptr;
            }
            else
            {
                cola = nullptr;
            }
        }
        else
        {
            actual->anterior->siguiente = actual->siguiente;
            if (actual->siguiente != nullptr)
            {
                actual->siguiente->anterior = actual->anterior;
            }
            else
            {
                cola = actual->anterior;
            }
        }

        std::cout << "Eliminando publicación con ID: " << actual->publicacion.getId() << std::endl;
        delete actual;
    }

    void mostrarTodasLasPublicaciones() const
    {
        NodoPublicacion *actual = cabeza;
        if (actual == nullptr)
        {
            std::cerr << "No hay publicaciones en la lista." << std::endl;
            return;
        }

        while (actual != nullptr)
        {
            std::cout << "ID: " << actual->publicacion.getId()
                      << ", Correo: " << actual->publicacion.getCorreo()
                      << ", Contenido: " << actual->publicacion.getContenido()
                      << ", Fecha: " << actual->publicacion.getFecha()
                      << ", Hora: " << actual->publicacion.getHora() << std::endl;
            actual = actual->siguiente;
        }
    }

    void mostrarPublicacionesPorCorreo(const std::string &correo)
    {
        NodoPublicacion *actual = cabeza;
        bool encontrado = false;

        while (actual != nullptr)
        {
            if (actual->publicacion.getCorreo() == correo)
            {
                std::cout << "ID: " << actual->publicacion.getId() << std::endl;
                std::cout << "Contenido: " << actual->publicacion.getContenido() << std::endl;
                std::cout << "Fecha: " << actual->publicacion.getFecha() << std::endl;
                std::cout << "Hora: " << actual->publicacion.getHora() << std::endl;
                std::cout << "--------------------------" << std::endl;
                encontrado = true;
            }
            actual = actual->siguiente;
        }

        if (!encontrado)
        {
            std::cout << "No se encontraron publicaciones para el usuario con correo: " << correo << std::endl;
        }
        else
        {
            std::cout << "Ingrese el ID de la publicación que desea borrar: ";
            int id_a_borrar;
            std::cin >> id_a_borrar;
            borrarPublicacionPorId(id_a_borrar);
        }
    }

    void cargarPublicacionesDesdeJson(const std::string &filename)
    {
        std::ifstream file(filename);
        if (file.is_open())
        {
            nlohmann::json jsonData;
            file >> jsonData;
            file.close();

            for (const auto &item : jsonData)
            {
                std::string correo = item["correo"];
                std::string contenido = item["contenido"];
                std::string fecha = item["fecha"];
                std::string hora = item["hora"];

                Publicacion nuevaPublicacion(siguienteId++, correo, contenido, fecha, hora);
                agregarPublicacion(nuevaPublicacion);
            }
        }
        else
        {
            std::cerr << "No se pudo abrir el archivo JSON." << std::endl;
        }
    }

private:
    NodoPublicacion *cabeza;
    NodoPublicacion *cola;
    int siguienteId;
};

std::string admin_correo = "admin@gmail.com";
std::string admin_contrasena = "EDD2S2024";

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    int opcion;
    ListaEmisor listaEmisor;
    PilaReceptor pilaReceptor;
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
                std::string dotFilename = "usuarios.dot";
                std::string imageFilename = "usuarios.png";
                std::string dotFilenameP = "publicaciones.dot";
                std::string imageFilenameP = "publicaciones.png";
                std::string receptor;
                std::string emisor;
                std::string estado = "PENDIENTE";
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
                        std::cout << "\n----------Menu Administrador----------\n";
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
                                listaUsuarios.cargarUsuariosDesdeJson("../" + archivo + ".json");

                                break;
                            case 2:
                                std::cout << "Opción seleccionada: Carga de relaciones.\n";
                                std::cout << "Ingrese el nombre del archivo: ";
                                std::cin >> archivo_R;
                                listaRelaciones.cargarRelacionesDesdeJson("../" + archivo_R + ".json");

                                break;
                            case 3:
                                std::cout << "Opción seleccionada: Carga de publicaciones.\n";
                                std::cout << "Ingrese el nombre del archivo: ";
                                std::cin >> archivo_P;
                                listaPublicaciones.cargarPublicacionesDesdeJson("../" + archivo_P + ".json");

                                break;
                            case 4:
                                std::cout << "Opción seleccionada: Gestionar usuarios.\n";
                                std::cout << "Ingrese el correo del usuario a borrar: ";
                                std::cin >> correo;
                                if (correo.empty())
                                {
                                    std::cerr << "No se ingresó un correo válido." << std::endl;
                                }
                                else
                                {
                                    listaUsuarios.borrarUsuarioPorCorreo(correo);
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
                                        listaUsuarios.generateDot(dotFilename);
                                        listaUsuarios.renderGraphviz(dotFilename, imageFilename);
                                        break;
                                    case 2:
                                        std::cout << "Opción seleccionada: Reporte de Relaciones de Amistad.\n";

                                        break;
                                    case 3:
                                        std::cout << "Opción seleccionada: Reporte de Publicaciones.\n";
                                        listaPublicaciones.generateDot(dotFilenameP);
                                        listaPublicaciones.renderGraphviz(dotFilenameP, imageFilenameP);
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
                            std::cout << "\n----------Menu Usuario----------\n";
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
                                    }
                                    else if (perfil_opcion == 2)
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
                                    int solicitudes_opcion;
                                    std::cout << "-----------------Solicitudes-----------------\n"
                                              << std::endl;
                                    std::cout << "1. Ver solicitudes\n";
                                    std::cout << "2. Enviar solicitud\n";
                                    std::cout << "Seleccione una opción (0 para regresar al menú principal): ";
                                    std::cin >> solicitudes_opcion;
                                    if (solicitudes_opcion == 1)
                                    {
                                        std::cout << "Solicitudes enviadas y recibidas\n";
                                        pilaReceptor.limpiarPila();
                                        pilaReceptor.buscarYApilarPendientes(correo, listaEmisor, listaRelaciones);
                                        listaEmisor.mostrarEmisores(listaRelaciones, correo);
                                        pilaReceptor.mostrarReceptores();
                                    }
                                    else if (solicitudes_opcion == 2)
                                    {
                                        std::cout << "Enviar solicitud\n";
                                        std::cout << "Ingrese el correo del receptor: ";
                                        std::cin >> receptor;
                                        listaEmisor.agregarEmisor(Emisor(correo, receptor, estado));
                                        break;
                                    }
                                    else
                                    {
                                        std::cout << "Opción no válida. Por favor, intente nuevamente.\n";
                                    }
                                    break;
                                case 3:
                                    int publicaciones_opcion;
                                    std::cout << "-------Publicaciones-------\n";
                                    std::cout << "1. Ver Todas\n";
                                    std::cout << "2. Crear publicación\n";
                                    std::cout << "3. Eliminar publicación\n";
                                    std::cout << "Seleccione una opción (0 para regresar al menú principal): ";
                                    std::cin >> publicaciones_opcion;
                                    if (publicaciones_opcion == 1)
                                    {
                                        std::cout << "Publicaciones\n";
                                        listaPublicaciones.mostrarTodasLasPublicaciones();
                                        break;
                                    }
                                    else if (publicaciones_opcion == 2)
                                    {
                                        std::cout << "Crear publicación\n";
                                        std::string contenido;
                                        std::string fecha;
                                        std::string hora;

                                        std::cout << "Introduce el contenido de la publicación: ";
                                        std::cin >> contenido;

                                        std::cout << "Introduce la fecha (YYYY-MM-DD): ";
                                        std::cin >> fecha;

                                        std::cout << "Introduce la hora (HH:MM): ";
                                        std::cin >> hora;

                                        // Crear la publicación
                                        listaPublicaciones.crearPublicacion(correo, contenido, fecha, hora);

                                        std::cout << "Publicación creada exitosamente.\n";
                                        break;
                                    }
                                    else if (publicaciones_opcion == 3)
                                    {
                                        std::cout << "Eliminar publicación\n";
                                        listaPublicaciones.mostrarPublicacionesPorCorreo(correo);

                                        break;
                                    }
                                    else
                                    {
                                        std::cout << "Opción no válida. Por favor, intente nuevamente.\n";
                                    }
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
                listaUsuarios.registrarUsuario(listaUsuarios);
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
