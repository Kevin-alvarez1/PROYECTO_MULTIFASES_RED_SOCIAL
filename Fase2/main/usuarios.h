#ifndef USUARIOS_H
#define USUARIOS_H

#include <QDialog>
#include "ListaUsuarios.h"
#include "listadoblepublicacion.h"

namespace Ui {
class Usuarios;
}
class Login;

class Usuarios : public QDialog
{
    Q_OBJECT

public:
    explicit Usuarios(std::string correoUsuario, ListaUsuarios* listaUsuarios, ListaDoblePublicacion listadoblepublicacion, QWidget *parent = nullptr);
    ~Usuarios();

private slots:
    void on_cerrar_sesion_btn_clicked();
    void on_buscar_correo_btn_clicked();
    void on_Eliminar_boton_clicked();

    void on_Modificar_boton_clicked();

private:
    Ui::Usuarios *ui;
    Login *login;
    ListaUsuarios *listaUsuarios;
    std::string correoActualUsuario_;
    ListaDoblePublicacion listadoblepublicacion;

};

#endif // USUARIOS_H
