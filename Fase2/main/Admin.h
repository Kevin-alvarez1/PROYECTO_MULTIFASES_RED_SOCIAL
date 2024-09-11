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
    void on_modificar_usuario_clicked(const std::string& correo, int fila);
    void on_eliminar_usuario_clicked(const std::string& correo);
    void on_buscar_usuario_admin_btn_clicked();
    void actualizarFilaEnTabla(const Usuario& usuario, int fila);
    void on_aplicar_orden_comboBox_orden_tabla_usuario_clicked();

private:
    Ui::Admin *ui;
    ListaUsuarios *listaUsuarios; // Cambiar a puntero
    Login *login;
};

#endif // ADMIN_H
