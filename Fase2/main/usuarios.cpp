#include "usuarios.h"
#include "ui_usuarios.h"
#include "login.h"
#include "QMessageBox"
Usuarios::Usuarios(ListaUsuarios *listaUsuarios, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Usuarios)
    , login(nullptr)
    , listaUsuarios(listaUsuarios)

{
    ui->setupUi(this);
}

Usuarios::~Usuarios()
{
    delete ui;
}

void Usuarios::on_cerrar_sesion_btn_clicked()
{
    if (!login) {
        login = new Login(listaUsuarios, this); // Pasar el puntero
    }

    login->show();
    this->hide();
}


// Usuarios.cpp
void Usuarios::on_buscar_correo_btn_clicked()
{
    std::string correo = ui->buscador_usuario_txt->text().toStdString();
    Usuario usuario = listaUsuarios->mostrarDatosPorCorreo(correo);

    if (usuario.getCorreo() == correo)  // Verifica que el usuario devuelto tiene el correo buscado
    {
        ui->buscador_usuario_nombre_txt->setText(QString::fromStdString(usuario.getNombre()));
        ui->buscador_usuario_apellido_txt->setText(QString::fromStdString(usuario.getApellido()));
        ui->buscador_usuario_correo_txt->setText(QString::fromStdString(usuario.getCorreo()));
        ui->buscador_usuario_fecha_nacimiento_txt->setText(QString::fromStdString(usuario.getFechaDeNacimiento()));
    }
    else
    {
        // Manejo de caso cuando el usuario no se encuentra
        ui->buscador_usuario_nombre_txt->clear();
        ui->buscador_usuario_apellido_txt->clear();
        ui->buscador_usuario_correo_txt->clear();
        ui->buscador_usuario_fecha_nacimiento_txt->clear();

        // Muestra un mensaje de advertencia al usuario
        QMessageBox::warning(this, "Error", "Usuario con correo " + QString::fromStdString(correo) + " no encontrado.");
    }
}
