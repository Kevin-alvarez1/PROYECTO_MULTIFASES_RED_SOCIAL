#include "usuarios.h"
#include "ui_usuarios.h"
#include "login.h"

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

