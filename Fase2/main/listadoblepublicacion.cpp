#include "listadoblepublicacion.h"
#include <iostream>
#include <fstream>
#include "json.hpp"

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
        }
    }
    else
    {
        std::cerr << "No se pudo abrir el archivo JSON." << std::endl;
    }
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

void ListaDoblePublicacion::mostrarTodasLasPublicaciones() const
{
    NodoPublicacion *actual = cabeza;
    while (actual != nullptr)
    {
        std::cout << "ID: " << actual->publicacion.getId() << " - Correo: " << actual->publicacion.getCorreo()
        << ", Contenido: " << actual->publicacion.getContenido()
        << ", Fecha: " << actual->publicacion.getFecha()
        << ", Hora: " << actual->publicacion.getHora() << std::endl;

        actual->publicacion.mostrarComentarios(); // Mostrar comentarios de la publicación

        actual = actual->siguiente;
    }
}

ListaDoblePublicacion::NodoPublicacion::NodoPublicacion(const Publicacion &publicacion) : publicacion(publicacion), siguiente(nullptr), anterior(nullptr) {}
