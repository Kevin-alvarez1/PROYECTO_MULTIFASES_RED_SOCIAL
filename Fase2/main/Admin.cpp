#include "admin.h"
#include "ui_admin.h"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include "admin.h"
#include "login.h"  // Asegúrate de incluir este archivo
#include <QMessageBox>

Admin::Admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Admin)
    , login(nullptr)  // Inicializa login a nullptr
{
    ui->setupUi(this);
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_Usuarios_boton_archivo_clicked()
{
    // Selecciona el archivo JSON con un diálogo de archivo
    QString filename = QFileDialog::getOpenFileName(this, "Seleccionar archivo JSON", "", "Archivos JSON (*.json)");

    if (!filename.isEmpty())
    {
        // Cargar usuarios desde el archivo JSON
        std::ifstream archivo(filename.toStdString());

        if (archivo.is_open())
        {
            listaUsuarios.cargarUsuariosDesdeJson(filename.toStdString());
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
    // Verificar si la ventana de login ya existe
    if (!login) {
        login = new Login(this); // Crear una instancia de Login con 'this' como parent
    }

    // Mostrar la ventana de login
    login->show();

    // Ocultar la ventana actual
    this->hide();
}
