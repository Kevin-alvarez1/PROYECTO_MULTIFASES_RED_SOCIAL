#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "ListaUsuarios.h" // Incluir el encabezado de ListaUsuarios

class Admin;
class Usuarios;
class ListaUsuarios;

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_InicioSesion_btn_clicked();

private:
    Ui::Login *ui;
    Admin *adminWindow;
    Usuarios *usuarioWindow;
    ListaUsuarios *listaUsuarios;
};

#endif // LOGIN_H
