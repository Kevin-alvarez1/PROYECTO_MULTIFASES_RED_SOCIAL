#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include "ListaUsuarios.h"

class Login;

namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(ListaUsuarios *listaUsuarios, QWidget *parent = nullptr);
    ~Admin();

private slots:
    void on_Usuarios_boton_archivo_clicked();
    void on_Publicaciones_boton_archivo_clicked();
    void on_Solicitudes_boton_archivo_clicked();
    void on_CerrarSesion_boton_2_clicked();

private:
    Ui::Admin *ui;
    ListaUsuarios *listaUsuarios; // Cambiar a puntero
    Login *login;
};

#endif // ADMIN_H
