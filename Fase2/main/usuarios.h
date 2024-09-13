#ifndef USUARIOS_H
#define USUARIOS_H

#include <QDialog>
#include "ListaUsuarios.h"
#include "listaDoblePublicacion.h"
#include "lista_solicitudes.h"

namespace Ui {
class Usuarios;
}
class Login;

class Usuarios : public QDialog
{
    Q_OBJECT

public:
    explicit Usuarios(std::string correoUsuario,  ListaUsuarios *listaUsuarios, ListaDoblePublicacion *listadoblepublicacion, ListaSolicitudes *lista_solicitudes, QWidget *parent = nullptr);
    ~Usuarios();

private slots:
    void on_cerrar_sesion_btn_clicked();
    void on_buscar_correo_btn_clicked();
    void on_Eliminar_boton_clicked();

    void on_Modificar_boton_clicked();

    void on_btnEnviarSolicitud_clicked(const std::string& correo);
    void on_btnCancelar_clicked(const std::string& correo);

    void on_actualizar_tablas_clicked();

private:
    Ui::Usuarios *ui;
    Login *login;
    std::string correoActualUsuario_;
    ListaUsuarios *listaUsuarios;
    ListaDoblePublicacion *listadoblepublicacion;
    ListaSolicitudes *lista_solicitudes;
};

#endif // USUARIOS_H
