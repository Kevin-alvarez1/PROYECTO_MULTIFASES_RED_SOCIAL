#include <QApplication>
#include <QMessageBox>
#include "login.h"
#include "ListaUsuarios.h"
#include "listaDoblePublicacion.h"
#include "lista_solicitudes.h"
#include "huffman.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include "grafo_no_dirigido.h"
#include <filesystem>

extern ListaUsuarios listaUsuarios;
extern GrafoNoDirigido grafoNoDirigido;
extern ListaDoblePublicacion listaDoblePublicacion;


// Declaración de la función calcularFrecuencias
void calcularFrecuencias(const std::string& nombreArchivo, int frecuencias[256]) {
    std::ifstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo) {
        std::cerr << "Error al abrir el archivo." << std::endl;
        return;
    }

    char caracter;
    while (archivo.get(caracter)) {
        frecuencias[(unsigned char)caracter]++;
    }

    archivo.close();
}

void cargarUsuariosDesdeArchivo(const std::string& nombreArchivo, ListaUsuarios& listaUsuarios) {
    std::ifstream archivoEntrada(nombreArchivo);
    if (!archivoEntrada.is_open()) {
        std::cerr << "Error al abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivoEntrada, linea)) {
        if (!linea.empty()) {
            Usuario usuario = Usuario::deserializar(linea);
            listaUsuarios.agregarUsuario(usuario);
        }
    }

    archivoEntrada.close();
}

void cargarAmigosDesdeArchivo(const std::string& nombreArchivo, GrafoNoDirigido& grafoNoDirigido) {
    std::ifstream archivoEntrada(nombreArchivo);
    if (!archivoEntrada.is_open()) {
        std::cerr << "Error al abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivoEntrada, linea)) {
        if (!linea.empty()) {
            Solicitud solicitud = Solicitud::deserializar(linea);
            grafoNoDirigido.insertarRelacion(solicitud.getEmisor(), solicitud.getReceptor()); // Agregar relación

        }
    }

    archivoEntrada.close();
}

void cargarSolicitudesEnviadasDesdeArchivo(const std::string& nombreArchivo, ListaSolicitudes& listaSolicitudes) {
    std::ifstream archivoEntrada(nombreArchivo);
    if (!archivoEntrada.is_open()) {
        std::cerr << "Error al abrir el archivo " << nombreArchivo << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivoEntrada, linea)) {
        if (!linea.empty()) {
            Solicitud solicitud = Solicitud::deserializar(linea);
            listaSolicitudes.enviarSolicitud(solicitud.getEmisor(),solicitud.getReceptor());

        }
    }

    archivoEntrada.close();
}


void cargarPublicacionesDesdeJson(const std::string &filename) {
    try {
        std::cout << "************ Leyendo el archivo: " << filename << " ************" << std::endl;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo JSON: " << filename << std::endl;
            return;
        }

        nlohmann::json jsonData;
        file >> jsonData;

        // Extraer campos del objeto JSON
        if (jsonData.contains("index") && jsonData.contains("data") &&
            jsonData["data"].contains("Publicacion:")) {

            // Extraer datos de la publicación
            const auto& publicacionData = jsonData["data"]["Publicacion:"]; // Accediendo al objeto "Publicacion:"
            std::string contenido = publicacionData["contenido"];
            std::string correo = publicacionData["correo"];
            std::string fecha = publicacionData["fecha"];
            std::string hora = publicacionData["hora"];
            int index = publicacionData["id"];

            // Crear una nueva publicación
            Publicacion nuevaPublicacion(index, correo, contenido, fecha, hora);

            std::cout << "Creando publicación con índice: " << nuevaPublicacion.getId() << std::endl;

            // Agregar comentarios si existen
            if (jsonData["data"].contains("Comentarios")) {
                for (const auto &comentarioItem : jsonData["data"]["Comentarios"]) {
                    std::string comentarioCorreo = comentarioItem["correo"];
                    std::string comentarioContenido = comentarioItem["contenido"];
                    std::string comentarioFecha = comentarioItem["fecha"];
                    std::string comentarioHora = comentarioItem["hora"];

                    Comentario nuevoComentario(comentarioContenido, nuevaPublicacion.getId(), comentarioCorreo, comentarioFecha, comentarioHora);
                    arbolComentarios_.insertar(nuevoComentario);
                }
            }

            // Agregar la publicación a la lista
            listaDoblePublicacion.agregarPublicacion(nuevaPublicacion);
            std::cout << "Publicación añadida: " << nuevaPublicacion.getContenido() << std::endl;
        } else {
            std::cerr << "Faltan campos en la publicación." << std::endl;
        }

        std::cout << "Contenido del JSON: " << jsonData.dump(4) << std::endl; // Esto imprime el JSON de forma legible
        file.close();
    }
    catch (const nlohmann::json::exception& e) {
        std::cerr << "Error al procesar el archivo JSON: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error general: " << e.what() << std::endl;
    }
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    SetConsoleOutputCP(CP_UTF8);

    try {
        // Crear instancias globales
        ListaUsuarios listaUsuarios;
        ListaSolicitudes listaSolicitudes;
        Huffman huffman;
        NodoHuffman* arbolHuffman = nullptr;


        // Descomprimir información al iniciar la aplicación
        huffman.descomprimir("datos.edd", arbolHuffman, "datos_temp.edd");

        // Definir el arreglo de frecuencias
        int frecuencias[256] = {0}; // Inicializa todas las frecuencias a 0

        // Calcular frecuencias de los caracteres en el archivo descomprimido
        calcularFrecuencias("datos_temp.edd", frecuencias);

        // Construir el árbol de Huffman a partir de las frecuencias
        arbolHuffman = huffman.construirArbol(frecuencias);

        // Cargar usuarios desde el archivo Usuarios.edd
        cargarUsuariosDesdeArchivo("Usuarios.edd", listaUsuarios);
        cargarAmigosDesdeArchivo("Amigos.edd", grafoNoDirigido);
        cargarSolicitudesEnviadasDesdeArchivo("Solicitudes_enviadas_recibidas.edd", listaSolicitudes);
        // Recorrer los bloques de la carpeta 'blockchain'
        std::string carpeta = "blockchain/blockchain/"; // Ajusta aquí según tu estructura de carpetas

        std::cout << "=====================================" << std::endl;

        // Comprobar si el directorio existe
        if (!std::filesystem::exists(carpeta)) {
            std::cerr << "El directorio de bloques no existe: " << carpeta << std::endl;
            return 1;  // Salir con error
        }

        // Usar filesystem para recorrer los archivos en el directorio
        for (const auto &entry : std::filesystem::directory_iterator(carpeta)) {
            std::filesystem::path filePath = entry.path();
            std::cout << "Archivo encontrado: " << filePath.filename().string() << std::endl; // Imprimir todos los archivos
            cargarPublicacionesDesdeJson(filePath.string());

        }


        std::cout << "=====================================" << std::endl;

        // Pasar las instancias a Login
        Login w(&listaUsuarios, &listaDoblePublicacion, &listaSolicitudes);
        w.show();

        return a.exec();
    }
    catch (const std::exception& e) {
        std::cerr << "Error general: " << e.what() << std::endl;
        return 1; // Indica que ocurrió un error.
    }
}

