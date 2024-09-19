#ifndef USUARIOS_H
#define USUARIOS_H

#include <QDialog>
#include "ListaUsuarios.h"
#include "listaDoblePublicacion.h"
#include "lista_solicitudes.h"
#include "pilareceptor.h"
#include "crearpublicacion.h"
#include <QComboBox>
#include <QLabel>

class CrearPublicacion;

namespace Ui {
class Usuarios;
}
class Login;
class Usuarios : public QDialog
{
    Q_OBJECT

public:
    explicit Usuarios(std::string correoUsuario, ListaUsuarios *listaUsuarios, ListaDoblePublicacion *listadoblepublicacion, ListaSolicitudes *lista_solicitudes, QWidget *parent = nullptr);
    ~Usuarios();
    ListaSolicitudes& obtenerListaSolicitudesEnviadas();
    ListaSolicitudes& obtenerListaSolicitudesRecibidas();
    std::vector<std::string> obtenerAmigos() const;
private slots:
    void on_cerrar_sesion_btn_clicked();
    void on_buscar_correo_btn_clicked();
    void on_Eliminar_boton_clicked();
    void on_Modificar_boton_clicked();
    void on_btnEnviarSolicitud_clicked(const std::string& correo);
    void on_btnCancelar_clicked(const std::string& correo);
    void on_actualizar_tablas_clicked();
    void on_btnAceptar_clicked(const std::string& correo);
    void on_btnRechazar_clicked(const std::string& correo);
    void on_fecha_filtro_publis_boton_clicked();
    void on_crear_nueva_publi_boton_clicked();
    void on_aplicar_orden_publis_boton_clicked();
    void on_generar_bst_reporte_boton_clicked();
    void actualizarPanelConImagen(const QString& imagePath);
private:
    Ui::Usuarios *ui;
    Login *login;
    std::string correoActualUsuario_;
    ListaUsuarios *listaUsuarios;
    ListaDoblePublicacion *listadoblepublicacion;
    ListaSolicitudes *lista_solicitudes;
    CrearPublicacion *CrearPublicacion;
};

#endif // USUARIOS_H
