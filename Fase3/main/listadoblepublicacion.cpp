#include "listaDoblePublicacion.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "arbolabb.h"
#include "grafo_no_dirigido.h"
#include "blockchain.h"

ListaDoblePublicacion listaDoblePublicacion;


extern ArbolBComentario arbolComentarios_;
extern Blockchain blockchain;

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

void ListaDoblePublicacion::guardarPublicacionesEnBloques(const std::string& directory) {
    // Asegúrate de que el directorio exista
    std::filesystem::create_directories(directory); // Crea el directorio si no existe

    std::ofstream file;

    // Crear un bloque por cada publicación
    NodoPublicacion* actual = cabeza;
    int chainSize = 0;  // Asegúrate de que este conteo comience desde 0
    while (actual != nullptr) {
        // Obtener los comentarios de la publicación
        std::vector<Comentario> comentarios = arbolComentarios_.getComentariosDePublicacion(actual->publicacion.getId());

        // Crear un objeto JSON para la publicación
        nlohmann::json publicacionJson;

        // Anidar la publicación en un objeto "Publicacion:"
        publicacionJson["Publicacion:"] = {
            {"contenido", actual->publicacion.getContenido()},
            {"correo", actual->publicacion.getCorreo()},
            {"fecha", actual->publicacion.getFecha()},
            {"hora", actual->publicacion.getHora()},
            {"id", actual->publicacion.getId()}
        };

        // Crear un array JSON para los comentarios
        nlohmann::json comentariosJson = nlohmann::json::array(); // Inicializa como un array
        for (const auto& comentario : comentarios) {
            nlohmann::json comentarioJson;
            comentarioJson["correo"] = comentario.getCorreo();
            comentarioJson["contenido"] = comentario.getComentario();
            comentarioJson["fecha"] = comentario.getFecha();
            comentarioJson["hora"] = comentario.getHora();
            comentariosJson.push_back(comentarioJson);
        }

        // Añadir los comentarios al objeto de la publicación
        publicacionJson["Comentarios"] = comentariosJson;

        // Convertir el JSON de la publicación (con comentarios) a string
        std::string publicacionConComentariosJSON = publicacionJson.dump(4); // Usa indentación de 4 espacios

        // Crear el bloque
        Block newBlock(chainSize++, publicacionConComentariosJSON, blockchain.getLastBlock().hash);
        newBlock.mineBlock(blockchain.difficulty);

        // Guardar el bloque en un archivo JSON
        std::string filename = directory + "/block_" + std::to_string(newBlock.index) + ".json";
        file.open(filename);
        if (file.is_open()) {
            file << newBlock.toJSON(); // Asegúrate de que este método incluya todo lo que necesites
            file.close();
        } else {
            std::cerr << "Error: No se pudo abrir el archivo " << filename << " para escribir." << std::endl;
        }

        // Avanzar al siguiente nodo
        actual = actual->siguiente;
    }
}


void ListaDoblePublicacion::cargarPublicacionesDesdeJson(const std::string &filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo JSON: " << filename << std::endl;
            return;
        }

        nlohmann::json jsonData;
        file >> jsonData;

        for (const auto &item : jsonData) {
            // Extraer datos de la publicación
            std::string correo = item["correo"];
            std::string contenido = item["contenido"];
            std::string fecha = item["fecha"];
            std::string hora = item["hora"];

            // Crear nueva publicación
            Publicacion nuevaPublicacion(siguienteId++, correo, contenido, fecha, hora);

            // Agregar comentarios
            if (item.contains("comentarios")) {
                for (const auto &comentarioItem : item["comentarios"]) {

                    // Extraer datos del comentario
                    std::string comentarioCorreo = comentarioItem["correo"];
                    std::string comentarioContenido = comentarioItem["comentario"];
                    std::string comentarioFecha = comentarioItem["fecha"];
                    std::string comentarioHora = comentarioItem["hora"];

                    // Crear nuevo comentario usando el ID de la publicación actual
                    Comentario nuevoComentario(comentarioContenido, nuevaPublicacion.getId(), comentarioCorreo, comentarioFecha, comentarioHora);

                    // Agregar comentario a la publicación
                    arbolComentarios_.insertar(nuevoComentario);
                    std::cout << "Obteniendo comentarios para la publicación ID: " << nuevaPublicacion.getId() << "comentario añadido con el id: " << nuevoComentario.getIdPublicacion() << std::endl;
                }
            }
            std::string carpeta = "blockchain"; // Nombre de la carpeta
            std::string directorioBloques = carpeta + "/blockchain"; // Crear un subdirectorio para los bloques

            // Agregar la nueva publicación a la lista
            agregarPublicacion(nuevaPublicacion);
            guardarPublicacionesEnBloques(directorioBloques);
        }

        file.close();
    }
    catch (const nlohmann::json::exception& e) {
        std::cerr << "Error al procesar el archivo JSON: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error general: " << e.what() << std::endl;
    }
}

void ListaDoblePublicacion::eliminarPublicacionPorId(int id) {
    NodoPublicacion* actual = cabeza; // Comenzamos desde la cabeza

    while (actual != nullptr) {
        if (actual->publicacion.getId() == id) { // Verificamos si el ID coincide
            if (actual->anterior != nullptr) { // Si no es el primer nodo
                actual->anterior->siguiente = actual->siguiente; // Actualizamos el siguiente del nodo anterior
            } else {
                cabeza = actual->siguiente; // Si es el primer nodo, actualizamos la cabeza
            }

            if (actual->siguiente != nullptr) { // Si no es el último nodo
                actual->siguiente->anterior = actual->anterior; // Actualizamos el anterior del nodo siguiente
            } else {
                cola = actual->anterior; // Si es el último nodo, actualizamos la cola
            }

            std::cout << "Depuración: Eliminando nodo de publicación con ID: " << actual->publicacion.getId() << std::endl;
            delete actual; // Liberamos la memoria del nodo
            return; // Salimos del método
        }
        actual = actual->siguiente; // Avanzamos al siguiente nodo
    }

    std::cout << "No se encontró publicación con ID: " << id << std::endl; // Mensaje si no se encuentra el ID
}

void ListaDoblePublicacion::eliminarPublicacionesPorCorreo(const std::string& correo) {
    NodoPublicacion* actual = cabeza; // Comenzamos desde la cabeza de la lista

    while (actual != nullptr) {
        if (actual->publicacion.getCorreo() == correo) {
            NodoPublicacion* nodoAEliminar = actual;

            if (nodoAEliminar->anterior != nullptr) {
                nodoAEliminar->anterior->siguiente = nodoAEliminar->siguiente;
            } else {
                cabeza = nodoAEliminar->siguiente;
            }

            if (nodoAEliminar->siguiente != nullptr) {
                nodoAEliminar->siguiente->anterior = nodoAEliminar->anterior;
            } else {
                cola = nodoAEliminar->anterior;
            }

            actual = nodoAEliminar->siguiente;

            std::cout << "Depuración: Eliminando publicación con ID: " << nodoAEliminar->publicacion.getId() << " y correo: " << nodoAEliminar->publicacion.getCorreo() << std::endl;
            delete nodoAEliminar;
        } else {
            actual = actual->siguiente;
        }
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

std::vector<Publicacion> ListaDoblePublicacion::mostrarPublicacionesYAmigos(
    const std::string &correo,
    const GrafoNoDirigido &grafoNoDirigido,
    ArbolABB &arbol,
    const std::string &orden) {

    std::vector<Publicacion> publicaciones_arbol; // Vector para almacenar las publicaciones
    publicaciones_arbol.clear();

    try {
        arbol = ArbolABB();

        // Obtener la lista de amigos del usuario
        int cantidadAmigos;
        std::string* amigos = grafoNoDirigido.obtenerAmigos(correo, cantidadAmigos);
        // Crear un arreglo dinámico para incluir el propio correo del usuario
        std::string* amigosConUsuario = new std::string[cantidadAmigos + 1];

        // Copiar amigos al nuevo arreglo y agregar el correo del usuario
        for (int i = 0; i < cantidadAmigos; i++) {
            amigosConUsuario[i] = amigos[i];
        }
        amigosConUsuario[cantidadAmigos] = correo; // Agregar el correo del usuario a la lista de amigos

        NodoPublicacion* actual = cabeza;
        while (actual) {
            // Comprobar si la publicación es del usuario o de un amigo
            for (int i = 0; i <= cantidadAmigos; i++) {
                if (actual->publicacion.getCorreo() == amigosConUsuario[i]) {
                    arbol.insertarPublicacion(actual->publicacion);
                    break; // Salir del bucle una vez que se encuentra una coincidencia
                }
            }
            actual = actual->siguiente;
        }

        // Almacenar las publicaciones en el orden deseado
        if (orden == "PreOrder") {
            std::vector<Publicacion> publicaciones_temp;
            arbol.recorrerPreOrder(publicaciones_temp);
            publicaciones_arbol.swap(publicaciones_temp);
        } else if (orden == "InOrder") {
            std::vector<Publicacion> publicaciones_temp;
            arbol.recorrerInOrder(publicaciones_temp);
            publicaciones_arbol.swap(publicaciones_temp);
        } else if (orden == "PostOrder") {
            std::vector<Publicacion> publicaciones_temp;
            arbol.recorrerPostOrder(publicaciones_temp);
            publicaciones_arbol.swap(publicaciones_temp);
        } else {
            std::cerr << "Orden no válido: " << orden << std::endl;
        }

        // Liberar la memoria del arreglo de amigos
        delete[] amigos; // Liberar el arreglo devuelto por obtenerAmigos
        delete[] amigosConUsuario; // Liberar el arreglo creado localmente
    } catch (const std::exception &e) {
        std::cerr << "Excepción capturada: " << e.what() << std::endl;
    }

    return publicaciones_arbol;
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

        actual = actual->siguiente;
    }
}

void ListaDoblePublicacion::agregarPublicacion(const Publicacion &publicacion) {
    NodoPublicacion *nuevoNodo = new NodoPublicacion(publicacion);

    if (cabeza == nullptr) {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
        std::cout << "Primera publicación añadida: " << publicacion.getContenido() << std::endl;
    } else {
        cola->siguiente = nuevoNodo;
        nuevoNodo->anterior = cola;
        cola = nuevoNodo;
        std::cout << "Publicación añadida: " << publicacion.getContenido() << std::endl;
    }
}




ListaDoblePublicacion::NodoPublicacion::NodoPublicacion(const Publicacion &publicacion) : publicacion(publicacion), siguiente(nullptr), anterior(nullptr) {}
