#include "usuarios.h"
#include "ui_usuarios.h"

Usuarios::Usuarios(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Usuarios)
{
    ui->setupUi(this);
}

Usuarios::~Usuarios()
{
    delete ui;
}
