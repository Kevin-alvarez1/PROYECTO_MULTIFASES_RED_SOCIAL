#include "login.h"
#include "./ui_login.h"
#include "admin.h"
#include "./ui_admin.h"
#include "usuarios.h"
#include "./ui_usuarios.h"
#include "registrarse.h"
#include "./ui_registrarse.h"
#include <QMessageBox>

Login::Login(ListaUsuarios *listaUsuarios, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Login),
    adminWindow(nullptr),
    usuarioWindow(nullptr),
    listaUsuarios(listaUsuarios)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_InicioSesion_btn_clicked()
{
    // Convertir QString a std::string
    std::string correo = ui->usuario_inicio_sesion_txt_area->text().toStdString();
    std::string contrasena = ui->contrasena_txt_area->text().toStdString();
    std::string admin_usuario_std = "admin@gmail.com";
    std::string admin_contraseña_std = "EDD2S2024";

    if (correo == admin_usuario_std && contrasena == admin_contraseña_std) {
        if (!adminWindow) {
            adminWindow = new Admin(listaUsuarios, this); // Pasar el puntero
        }
        adminWindow->show();
        this->hide();
    } else {
        if (listaUsuarios->buscarUsuarioPorCorreoyContrasena(correo, contrasena)){
            if (!usuarioWindow) {
                usuarioWindow = new Usuarios(listaUsuarios);
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
    if (!usuarioWindow) {
        registrarseWindow = new Registrarse(listaUsuarios);
    }
    registrarseWindow->show();
    this->hide();
}

