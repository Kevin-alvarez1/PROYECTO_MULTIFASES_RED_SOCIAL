#include "login.h"
#include "./ui_login.h"
#include "admin.h"
#include "./ui_admin.h"
#include "usuarios.h"
#include "./ui_usuarios.h"
#include <QMessageBox>
#include <iostream>

Login::Login(QWidget *parent)
    : QMainWindow(parent), // Inicializar el padre
    ui(new Ui::Login),
    adminWindow(nullptr),
    usuarioWindow(nullptr)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_InicioSesion_btn_clicked()
{
    QString admin_usuario = "admin@gmail.com";
    QString admin_contraseña = "EDD2S2024";

    QString usuario = ui->usuario_inicio_sesion_txt_area->text();
    QString contraseña = ui->contrasena_txt_area->text();

    // Convertir QString a std::string
    std::string usuario_std = usuario.toStdString();
    std::string contraseña_std = contraseña.toStdString();
    std::string admin_usuario_std = admin_usuario.toStdString();
    std::string admin_contraseña_std = admin_contraseña.toStdString();

    if (usuario_std == admin_usuario_std && contraseña_std == admin_contraseña_std) {
        if (!adminWindow) {
            adminWindow = new Admin();
        }
        adminWindow->show();
        this->hide();
    } else {
        if (listaUsuarios->buscarUsuarioPorCorreoyContrasena(usuario_std, contraseña_std)) {
            if (!usuarioWindow) {
                usuarioWindow = new Usuarios();
             }
             usuarioWindow->show();
             this->hide();
        } else {
            QMessageBox::warning(this, "Error", "Usuario o contraseña incorrectos.");
        }
    }
}


