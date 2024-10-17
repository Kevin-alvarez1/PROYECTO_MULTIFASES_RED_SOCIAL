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

extern ListaUsuarios listaUsuarios;
extern GrafoNoDirigido grafoNoDirigido;

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
            listaUsuarios.agregarUsuario(usuario); // Asegúrate de que tengas un método para agregar usuarios
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
            Solicitud solicitud = Solicitud::deserializar(linea);  // Asegúrate de que este método esté definido
            grafoNoDirigido.insertarRelacion(solicitud.getEmisor(), solicitud.getReceptor()); // Agregar relación

        }
    }

    archivoEntrada.close();
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    SetConsoleOutputCP(CP_UTF8);

    // Crear instancias globales
    ListaUsuarios listaUsuarios;
    ListaDoblePublicacion listaDoblePublicacion;
    ListaSolicitudes listaSolicitudes;
    Huffman huffman;  // Instancia de la clase Huffman
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
    // Pasar las instancias a Login
    Login w(&listaUsuarios, &listaDoblePublicacion, &listaSolicitudes);

    w.show();

    return a.exec();
}
