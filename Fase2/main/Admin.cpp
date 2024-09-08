#include "admin.h"
#include "ui_admin.h"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include "login.h"

Admin::Admin(ListaUsuarios *listaUsuarios, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::Admin),
    listaUsuarios(listaUsuarios),
    login(nullptr)
{
    ui->setupUi(this);
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_Usuarios_boton_archivo_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Seleccionar archivo JSON", "", "Archivos JSON (*.json)");

    if (!filename.isEmpty())
    {
        std::ifstream archivo(filename.toStdString());

        if (archivo.is_open())
        {
            listaUsuarios->cargarUsuariosDesdeJson(filename.toStdString());
            QMessageBox::information(this, "Cargar usuarios", "Usuarios cargados exitosamente.");
        }
        else
        {
            QMessageBox::warning(this, "Error", "No se pudo abrir el archivo.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "No se seleccionó ningún archivo.");
    }
}

void Admin::on_Solicitudes_boton_archivo_clicked()
{
    // Implementar la funcionalidad para solicitudes
}

void Admin::on_Publicaciones_boton_archivo_clicked()
{
    // Implementar la funcionalidad para publicaciones
}

void Admin::on_CerrarSesion_boton_2_clicked()
{
    if (!login) {
        login = new Login(listaUsuarios, this); // Pasar el puntero
    }

    login->show();
    this->hide();
}
