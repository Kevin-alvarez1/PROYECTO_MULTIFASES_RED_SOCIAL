#ifndef USUARIOS_H
#define USUARIOS_H

#include <QDialog>
#include "ListaUsuarios.h"

namespace Ui {
class Usuarios;
}
class Login;

class Usuarios : public QDialog
{
    Q_OBJECT

public:
    explicit Usuarios(ListaUsuarios *listaUsuarios, QWidget *parent = nullptr);
    ~Usuarios();

private slots:
    void on_cerrar_sesion_btn_clicked();

private:
    Ui::Usuarios *ui;
    Login *login;
    ListaUsuarios *listaUsuarios; // Cambiar a puntero

};

#endif // USUARIOS_H
