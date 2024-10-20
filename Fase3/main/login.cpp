#include "login.h"
#include "./ui_login.h"
#include "Admin.h"
#include "./ui_Admin.h"
#include "usuarios.h"
#include "./ui_usuarios.h"
#include "registrarse.h"
#include "./ui_registrarse.h"
#include <QMessageBox>
#include <QCloseEvent>
#include "huffman.h"
#include "grafo_no_dirigido.h"
extern Huffman huffman;
extern GrafoNoDirigido grafoNoDirigido;

Login::Login(ListaUsuarios *listaUsuarios, ListaDoblePublicacion *listadoblepublicacion, ListaSolicitudes *lista_solicitudes, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Login),
    adminWindow(nullptr),
    usuarioWindow(nullptr),
    registrarseWindow(nullptr),
    listaUsuarios(listaUsuarios),
    listadoblepublicacion(listadoblepublicacion),
    lista_solicitudes(lista_solicitudes)
{
    ui->setupUi(this);
    connect(this, &Login::cerrarSesion, this, &Login::guardarUsuarios);

}

Login::~Login()
{
    delete ui;
}

void Login::on_InicioSesion_btn_clicked()
{
    std::string correo = ui->usuario_inicio_sesion_txt_area->text().toStdString();
    std::string contrasena = ui->contrasena_txt_area->text().toStdString();
    std::string admin_usuario_std = "admin@gmail.com";
    std::string admin_contraseña_std = "EDD2S2024";

    if (correo == admin_usuario_std && contrasena == admin_contraseña_std) {
        if (!adminWindow) {
            adminWindow = new Admin(listaUsuarios, listadoblepublicacion, lista_solicitudes, this);
        }
        adminWindow->show();
        this->hide();
    } else {
        if (listaUsuarios->buscarUsuarioPorCorreoyContrasena(correo, contrasena)) {
            if (!usuarioWindow) {
                usuarioWindow = new Usuarios(correo, listaUsuarios, listadoblepublicacion, lista_solicitudes, this);
            }
            usuarioWindow->show();
            this->hide();
        } else {
            QMessageBox::warning(this, "Error", "Usuario o contraseña incorrectos.");
        }
    }
}

void Login::on_Registrarse_btn_clicked()
{
    if (!registrarseWindow) {
        registrarseWindow = new Registrarse(listaUsuarios, listadoblepublicacion, lista_solicitudes, this);

    }
    registrarseWindow->show();
    this->hide();
}


void Login::closeEvent(QCloseEvent *event) {
    // Si deseas confirmar el cierre de sesión con el usuario:
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Cerrar sesión", "¿Estás seguro de que quieres cerrar sesión?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit cerrarSesion();  // Emitir la señal de cierre de sesión
        event->accept();  // Cerrar la ventana
    } else {
        event->ignore();  // Ignorar el evento de cierre
    }
}

void Login::calcularFrecuencias(const std::string& nombreArchivo, int frecuencias[256]) {
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

void Login::guardarUsuarios() {
    // Guardar amigos y solicitudes
    grafoNoDirigido.guardarAmigos();
    lista_solicitudes->guardarSolicitudesEnviadas();

    // Serializar y guardar usuarios
    std::vector<Usuario> usuarios = listaUsuarios->obtenerUsuariosEnOrden("InOrder");
    std::cout << "Número de usuarios: " << usuarios.size() << std::endl; // Verificación de cantidad

    if (usuarios.empty()) {
        QMessageBox::warning(nullptr, "Advertencia", "No hay usuarios para guardar.");
        return;
    }

    std::ofstream archivoSalida("Usuarios.edd");
    if (!archivoSalida.is_open()) {
        std::cerr << "Error al abrir el archivo Usuarios.edd" << std::endl;
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo para guardar los datos.");
        return;
    }

    for (const Usuario& usuario : usuarios) {
        std::string datosUsuario = usuario.serializar();
        if (datosUsuario.empty()) {
            std::cerr << "Error: Serialización de usuario vacía." << std::endl;
        } else {
            std::cout << "Serializando usuario: " << datosUsuario << std::endl;
            archivoSalida << datosUsuario << std::endl;
        }
    }

    archivoSalida.close(); // Cerrar el archivo

    // Definir el arreglo de frecuencias para la compresión
    int frecuencias[256] = {0}; // Array para almacenar las frecuencias de cada carácter

    // --- Comprimir Usuarios.edd ---
    calcularFrecuencias("Usuarios.edd", frecuencias); // Calcular las frecuencias del archivo
    NodoHuffman* arbolHuffman = huffman.construirArbol(frecuencias); // Construir el árbol de Huffman
    if (!arbolHuffman) {
        QMessageBox::warning(nullptr, "Error", "No se pudo construir el árbol de Huffman para Usuarios.edd.");
        return;
    }
    if (!huffman.comprimir("Usuarios.edd", "usuarios_comprimidos.edd")) {
        QMessageBox::warning(nullptr, "Error", "No se pudo comprimir el archivo Usuarios.edd.");
    } else {
        std::cout << "Compresión exitosa. Archivo comprimido guardado como usuarios_comprimidos.edd" << std::endl;
    }

    // --- Comprimir Amigos.edd ---
    std::fill(std::begin(frecuencias), std::end(frecuencias), 0); // Reiniciar las frecuencias
    calcularFrecuencias("Amigos.edd", frecuencias); // Calcular frecuencias para Amigos.edd
    arbolHuffman = huffman.construirArbol(frecuencias); // Construir árbol para Amigos.edd
    if (!arbolHuffman) {
        QMessageBox::warning(nullptr, "Error", "No se pudo construir el árbol de Huffman para Amigos.edd.");
        return;
    }
    if (!huffman.comprimir("Amigos.edd", "amigos_comprimidos.edd")) {
        QMessageBox::warning(nullptr, "Error", "No se pudo comprimir el archivo Amigos.edd.");
    } else {
        std::cout << "Compresión exitosa. Archivo comprimido guardado como amigos_comprimidos.edd" << std::endl;
    }

    // --- Comprimir Solicitudes_enviadas_recibidas.edd ---
    std::fill(std::begin(frecuencias), std::end(frecuencias), 0); // Reiniciar las frecuencias
    calcularFrecuencias("Solicitudes_enviadas_recibidas.edd", frecuencias); // Calcular frecuencias para el archivo
    arbolHuffman = huffman.construirArbol(frecuencias); // Construir árbol de Huffman
    if (!arbolHuffman) {
        QMessageBox::warning(nullptr, "Error", "No se pudo construir el árbol de Huffman para Solicitudes_enviadas_recibidas.edd.");
        return;
    }
    if (!huffman.comprimir("Solicitudes_enviadas_recibidas.edd", "solicitudes_comprimidas.edd")) {
        QMessageBox::warning(nullptr, "Error", "No se pudo comprimir el archivo Solicitudes_enviadas_recibidas.edd.");
    } else {
        std::cout << "Compresión exitosa. Archivo comprimido guardado como solicitudes_comprimidas.edd" << std::endl;
    }

    // --- Guardar blockchain de publicaciones y comentarios ---

    std::string carpeta = "blockchain"; // Nombre de la carpeta
    std::string directorioBloques = carpeta + "/blockchain"; // Crear un subdirectorio para los bloques

    listadoblepublicacion->guardarPublicacionesEnBloques(directorioBloques);


}


