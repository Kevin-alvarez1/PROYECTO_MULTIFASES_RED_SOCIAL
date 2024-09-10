#include "Admin.h"
#include "./ui_Admin.h"
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

void Admin::on_buscar_usuario_admin_btn_clicked()
{
    // Obtener el correo ingresado por el usuario
    std::string correo = ui->buscar_usuario_admin_txt_area->text().toStdString();

    // Buscar el usuario con el correo ingresado
    Usuario usuario = listaUsuarios->mostrarDatosPorCorreo(correo);

    // Limpiar la tabla antes de mostrar los resultados
    ui->tabla_buscar_admin->setRowCount(0);  // Limpia las filas actuales de la tabla

    if (usuario.getCorreo() == correo)
    {
        bool encontrado = false;
        int filas = ui->tabla_buscar_admin->rowCount();

        // Comprobar si el usuario ya está en la tabla
        for (int i = 0; i < filas; ++i) {
            QTableWidgetItem* item = ui->tabla_buscar_admin->item(i, 2); // Columna de correo
            if (item && item->text().toStdString() == correo) {
                encontrado = true;
                break;
            }
        }

        if (!encontrado) {
            // Crear una nueva fila en la tabla
            int fila = ui->tabla_buscar_admin->rowCount();
            ui->tabla_buscar_admin->insertRow(fila);

            // Insertar los datos en las celdas correspondientes
            ui->tabla_buscar_admin->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(usuario.getNombre())));
            ui->tabla_buscar_admin->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(usuario.getApellido())));
            ui->tabla_buscar_admin->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(usuario.getCorreo())));
            ui->tabla_buscar_admin->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(usuario.getFechaDeNacimiento())));

            // Crear botones de Modificar y Eliminar
            QPushButton* btnModificar = new QPushButton("Modificar");
            QPushButton* btnEliminar = new QPushButton("Eliminar");

            // Añadir los botones a las columnas correspondientes
            ui->tabla_buscar_admin->setCellWidget(fila, 4, btnModificar);
            ui->tabla_buscar_admin->setCellWidget(fila, 5, btnEliminar);

            // Conectar los botones a sus respectivos slots
            connect(btnModificar, &QPushButton::clicked, [this, correo]() {
                this->on_modificar_usuario_clicked(correo);
            });
            connect(btnEliminar, &QPushButton::clicked, [this, correo]() {
                this->on_eliminar_usuario_clicked(correo);
            });
        }
    }
    else
    {
        // Opcional: Muestra un mensaje de advertencia si el usuario no se encuentra
        QMessageBox::warning(this, "Error", "Usuario con correo " + QString::fromStdString(correo) + " no encontrado.");
    }
}
void Admin::on_modificar_usuario_clicked(const std::string& correo)
{
    // Lógica para modificar el usuario
    // Puedes abrir un diálogo para modificar los datos del usuario
    QMessageBox::information(this, "Modificar Usuario", "Modificar usuario con correo: " + QString::fromStdString(correo));
}

void Admin::on_eliminar_usuario_clicked(const std::string& correo)
{
    // Lógica para eliminar el usuario
    // Asegúrate de eliminar el usuario de la tabla y de la lista de usuarios
    listaUsuarios->borrarUsuarioPorCorreo(correo);

    // Eliminar la fila correspondiente de la tabla
    for (int i = 0; i < ui->tabla_buscar_admin->rowCount(); ++i) {
        QTableWidgetItem* item = ui->tabla_buscar_admin->item(i, 2); // Columna de correo
        if (item && item->text().toStdString() == correo) {
            ui->tabla_buscar_admin->removeRow(i);
            break;
        }
    }
}

