#include "listaDoblePublicacion.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "arbolabb.h"
#include "matrizdispersa.h"
extern ArbolABB arbolABB;
extern MatrizDispersa matrizDispersa;

ListaDoblePublicacion::ListaDoblePublicacion() : cabeza(nullptr), cola(nullptr), siguienteId(1)
{
    std::cout << "Depuración: Lista de publicaciones creada." << std::endl;
}

ListaDoblePublicacion::~ListaDoblePublicacion()
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

void ListaDoblePublicacion::cargarPublicacionesDesdeJson(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "No se pudo abrir el archivo JSON: " << filename << std::endl;
        return;
    }

    try
    {
        nlohmann::json jsonData;
        file >> jsonData;

        for (const auto &item : jsonData)
        {
            std::string correo = item["correo"];
            std::string contenido = item["contenido"];
            std::string fecha = item["fecha"];
            std::string hora = item["hora"];

            Publicacion nuevaPublicacion(siguienteId++, correo, contenido, fecha, hora);

            // Agregar comentarios
            for (const auto &comentarioItem : item["comentarios"])
            {
                std::string comentarioCorreo = comentarioItem["correo"];
                std::string comentarioContenido = comentarioItem["comentario"];
                std::string comentarioFecha = comentarioItem["fecha"];
                std::string comentarioHora = comentarioItem["hora"];

                Comentario nuevoComentario(comentarioCorreo, comentarioContenido, comentarioFecha, comentarioHora);
                nuevaPublicacion.agregarComentario(nuevoComentario);
            }

            agregarPublicacion(nuevaPublicacion);
            arbolABB.insertarPublicacion(nuevaPublicacion);
        }
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cerr << "Error al procesar el archivo JSON: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error general: " << e.what() << std::endl;
    }
}

int ListaDoblePublicacion::obtenerNuevoId() const
{
    int maxId = 0;
    NodoPublicacion* actual = cabeza;
    while (actual != nullptr)
    {
        if (actual->publicacion.getId() > maxId)
        {
            maxId = actual->publicacion.getId();
        }
        actual = actual->siguiente;
    }
    return maxId + 1;
}


void ListaDoblePublicacion::generateDot(const std::string &filename) {
    std::string dotFilename = filename + ".dot";

    std::ofstream file(dotFilename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo para escribir el DOT." << std::endl;
        return;
    }

    file << "digraph G {\n";
    file << "  node [shape=record];\n";

    NodoPublicacion* actual = cabeza;
    int nodoId = 0;

    while (actual != nullptr) {
        file << "  nodo" << nodoId << " [label=\"{"
             << actual->publicacion.getCorreo() << " | "
             << actual->publicacion.getContenido() << "}\"];\n";

        if (actual->siguiente != nullptr) {
            file << "  nodo" << nodoId << " -> nodo" << nodoId + 1 << ";\n";
            file << "  nodo" << nodoId + 1 << " -> nodo" << nodoId << ";\n"; // Doble enlace
        }

        actual = actual->siguiente;
        nodoId++;
    }

    file << "}\n";
    file.close();

    std::string pngFilename = filename + ".png";
    crearPNG(dotFilename, pngFilename);
}

void ListaDoblePublicacion::crearPNG(const std::string &dotFilename, const std::string &pngFilename) {
    std::string command = "dot -Tpng " + dotFilename + " -o " + pngFilename;
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error al ejecutar el comando Graphviz para generar el PNG." << std::endl;
    }
}

void ListaDoblePublicacion::mostrarPublicacionesYAmigos(const std::string &correo, const MatrizDispersa &matriz)
{
    // Muestra las publicaciones del usuario dado
    mostrarPublicacionesPorCorreo(correo);

    // Obtiene la lista de amigos del usuario
    std::vector<std::string> amigos = matriz.obtenerAmigos(correo);

    // Muestra las publicaciones de cada amigo
    for (const auto &amigo : amigos)
    {
        std::cout << "Publicaciones de " << amigo << ":" << std::endl;
        mostrarPublicacionesPorCorreo(amigo);
    }
}


void ListaDoblePublicacion::mostrarPublicacionesPorCorreo(const std::string& correo) const
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
}




void ListaDoblePublicacion::mostrarPublicacion(const Publicacion& publicacion) const
{
    std::cout << "ID: " << publicacion.getId() << " - Correo: " << publicacion.getCorreo()
    << ", Contenido: " << publicacion.getContenido()
    << ", Fecha: " << publicacion.getFecha()
    << ", Hora: " << publicacion.getHora() << std::endl;

    // Mostrar comentarios de la publicación
    publicacion.mostrarComentarios(); // Asumiendo que Publicacion tiene un método para mostrar comentarios
}

void ListaDoblePublicacion::mostrarTodasLasPublicaciones() const {
    NodoPublicacion *actual = cabeza;
    while (actual != nullptr) {
        std::cout << "ID: " << actual->publicacion.getId()
        << " - Correo: " << actual->publicacion.getCorreo()
        << ", Contenido: " << actual->publicacion.getContenido()
        << ", Fecha: " << actual->publicacion.getFecha()
        << ", Hora: " << actual->publicacion.getHora()
        << std::endl;

        actual->publicacion.mostrarComentarios(); // Mostrar comentarios de la publicación

        actual = actual->siguiente;
    }
}

void ListaDoblePublicacion::agregarPublicacion(const Publicacion &publicacion)
{
    NodoPublicacion *nuevoNodo = new NodoPublicacion(publicacion);

    if (cabeza == nullptr)
    {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
    }
    else
    {
        cola->siguiente = nuevoNodo;
        nuevoNodo->anterior = cola;
        cola = nuevoNodo;
    }
}



ListaDoblePublicacion::NodoPublicacion::NodoPublicacion(const Publicacion &publicacion) : publicacion(publicacion), siguiente(nullptr), anterior(nullptr) {}
