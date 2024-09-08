#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include "ListaUsuarios.h"

class Login;  // Declaración adelantada

namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

private slots:
    void on_Usuarios_boton_archivo_clicked();
    void on_Publicaciones_boton_archivo_clicked();
    void on_Solicitudes_boton_archivo_clicked();
    void on_CerrarSesion_boton_2_clicked();

private:
    Ui::Admin *ui;
    ListaUsuarios listaUsuarios; // Instancia de la lista de usuarios
    Login *login; // Ventana para el administrador
};

#endif // ADMIN_H
