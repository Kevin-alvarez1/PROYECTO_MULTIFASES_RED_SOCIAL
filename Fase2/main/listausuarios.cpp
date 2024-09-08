#include "listausuarios.h"
#include <iostream>
ListaUsuarios::ListaUsuarios() : cabeza(nullptr), cola(nullptr) {}

ListaUsuarios::~ListaUsuarios()
{
    Nodo *actual = cabeza;
    while (actual != nullptr)
    {
        Nodo *temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
}

void ListaUsuarios::agregarUsuario(const Usuario &usuario)
{
    if (usuarioDuplicado(usuario.getCorreo()))
    {
        std::cerr << "Usuario con correo " << usuario.getCorreo() << " ya existe." << std::endl;
        return;
    }

    Nodo *nuevoNodo = new Nodo(usuario);

    if (cabeza == nullptr)
    {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
    }
    else
    {
        cola->siguiente = nuevoNodo;
        cola = nuevoNodo;
    }
}

bool ListaUsuarios::usuarioDuplicado(const std::string &correo) const
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

void ListaUsuarios::cargarUsuariosDesdeJson(const std::string &nombreArchivo)
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
            std::string nombre = limpiarCadena(item.at("nombres").get<std::string>());
            std::string apellido = limpiarCadena(item.at("apellidos").get<std::string>());
            std::string fecha_de_nacimiento = limpiarCadena(item.at("fecha_de_nacimiento").get<std::string>());
            std::string correo = limpiarCadena(item.at("correo").get<std::string>());
            std::string contrasena = limpiarCadena(item.at("contraseña").get<std::string>());

            Usuario usuario(nombre, apellido, fecha_de_nacimiento, correo, contrasena);
            agregarUsuario(usuario);
            mostrarDatosPorCorreo(correo);

        }
    }
    catch (const nlohmann::json::exception &e)
    {
        std::cerr << "Error al procesar el archivo JSON: " << e.what() << std::endl;
    }
}

void ListaUsuarios::mostrarDatosPorCorreo(const std::string &correo) const
{
    Nodo *actual = cabeza;
    while (actual != nullptr)
    {
        if (actual->usuario.getCorreo() == correo)
        {
            std::cout << "Usuario encontrado:" << std::endl;
            std::cout << "Nombre: " << actual->usuario.getNombre() << std::endl;
            std::cout << "Correo: " << actual->usuario.getCorreo() << std::endl;
            std::cout << "Contraseña: " << actual->usuario.getContrasena() << std::endl;
            return;
        }
        actual = actual->siguiente;
    }
    std::cerr << "Usuario con correo " << correo << " no encontrado." << std::endl;
}

void ListaUsuarios::borrarUsuarioPorCorreo(const std::string &correo)
{
    if (cabeza == nullptr)
    {
        std::cerr << "La lista está vacía." << std::endl;
        return;
    }

    if (cabeza->usuario.getCorreo() == correo)
    {
        Nodo *temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
        return;
    }

    Nodo *actual = cabeza;
    Nodo *anterior = nullptr;

    while (actual != nullptr && actual->usuario.getCorreo() != correo)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == nullptr)
    {
        std::cerr << "Usuario con correo " << correo << " no encontrado." << std::endl;
        return;
    }

    anterior->siguiente = actual->siguiente;
    delete actual;
}

std::string ListaUsuarios::limpiarCadena(const std::string &str)
{
    std::string resultado = str;
    resultado.erase(0, resultado.find_first_not_of(" \n\r\t\""));
    resultado.erase(resultado.find_last_not_of(" \n\r\t\"") + 1);
    return resultado;
}

void ListaUsuarios::generateDot(const std::string &filename) const
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
    }
    else
    {
        std::cerr << "No se pudo abrir el archivo" << std::endl;
    }
}

bool ListaUsuarios::buscarUsuarioPorCorreoyContrasena(const std::string &correo, const std::string &contrasena) const
{
    std::cout << "=================================== " << std::endl;

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
    std::cout << "=================================== " << std::endl;

    return false;
}


void ListaUsuarios::renderGraphviz(const std::string &dotFilename, const std::string &imageFilename) const
{
    std::string command = "dot -Tpng " + dotFilename + " -o " + imageFilename;
    system(command.c_str());
}
