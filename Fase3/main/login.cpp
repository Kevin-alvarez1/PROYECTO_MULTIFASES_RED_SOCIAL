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

void Login::guardarUsuarios() {
    std::vector<Usuario> usuarios = listaUsuarios->obtenerUsuariosEnOrden("InOrder");
    std::cout << "Número de usuarios: " << usuarios.size() << std::endl; // Verificación de cantidad
    grafoNoDirigido.guardarAmigos();

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

    if (!huffman.comprimir("Usuarios.edd", "datos_comprimidos.edd")) {
        QMessageBox::warning(nullptr, "Error", "No se pudo comprimir los datos.");
    }

}

