#include "Admin.h"
#include "./ui_Admin.h"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include "login.h"
#include <QInputDialog>

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
    Usuario* usuario = listaUsuarios->buscarUsuarioPorCorreo(correo);
    ui->tabla_buscar_admin->setRowCount(0);

    if (usuario)
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
            ui->tabla_buscar_admin->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(usuario->getNombre())));
            ui->tabla_buscar_admin->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(usuario->getApellido())));
            ui->tabla_buscar_admin->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(usuario->getCorreo())));
            ui->tabla_buscar_admin->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(usuario->getFechaDeNacimiento())));

            // Crear botones de Modificar y Eliminar
            QPushButton* btnModificar = new QPushButton("Modificar");
            QPushButton* btnEliminar = new QPushButton("Eliminar");

            // Añadir los botones a las columnas correspondientes
            ui->tabla_buscar_admin->setCellWidget(fila, 4, btnModificar);
            ui->tabla_buscar_admin->setCellWidget(fila, 5, btnEliminar);

            // Conectar los botones a sus respectivos slots
            connect(btnModificar, &QPushButton::clicked, [this, usuario, fila]() {
                // Pasar el usuario y la fila para poder actualizar
                this->on_modificar_usuario_clicked(usuario->getCorreo(), fila);
            });
            connect(btnEliminar, &QPushButton::clicked, [this, usuario]() {
                this->on_eliminar_usuario_clicked(usuario->getCorreo());
            });
        }
    }
    else
    {
        // Opcional: Muestra un mensaje de advertencia si el usuario no se encuentra
        QMessageBox::warning(this, "Error", "Usuario con correo " + QString::fromStdString(correo) + " no encontrado.");
    }
}

void Admin::actualizarFilaEnTabla(const Usuario& usuario, int fila)
{
    // Asegúrate de que la fila existe
    if (fila < 0 || fila >= ui->tabla_buscar_admin->rowCount()) {
        qDebug() << "Fila no válida para actualizar.";
        return;
    }

    // Actualizar los datos de la fila en la tabla
    ui->tabla_buscar_admin->setItem(fila, 0, new QTableWidgetItem(QString::fromStdString(usuario.getNombre())));
    ui->tabla_buscar_admin->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(usuario.getApellido())));
    ui->tabla_buscar_admin->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(usuario.getCorreo())));
    ui->tabla_buscar_admin->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(usuario.getFechaDeNacimiento())));

    QPushButton* btnModificar = new QPushButton("Modificar");
    QPushButton* btnEliminar = new QPushButton("Eliminar");

    // Añadir los botones a las columnas correspondientes
    ui->tabla_buscar_admin->setCellWidget(fila, 4, btnModificar);
    ui->tabla_buscar_admin->setCellWidget(fila, 5, btnEliminar);

    // Conectar los botones a sus respectivos slots
    connect(btnModificar, &QPushButton::clicked, [this, usuario, fila]() {
        this->on_modificar_usuario_clicked(usuario.getCorreo(), fila);
    });
    connect(btnEliminar, &QPushButton::clicked, [this, usuario]() {
        this->on_eliminar_usuario_clicked(usuario.getCorreo());
    });
}

void Admin::on_modificar_usuario_clicked(const std::string& correo, int fila)
{
    // Buscar al usuario con el correo proporcionado
    Usuario* usuario = listaUsuarios->buscarUsuarioPorCorreo(correo);

    if (!usuario) {
        // Mostrar un mensaje si el usuario no fue encontrado
        QMessageBox::warning(this, "Error", "Usuario con correo: " + QString::fromStdString(correo) + " no encontrado.");
        return;
    }

    // Mostrar ventanas emergentes para obtener los nuevos datos
    bool ok;

    QString nuevoNombre = QInputDialog::getText(this, "Modificar Nombre",
                                                "Nuevo Nombre:",
                                                QLineEdit::Normal,
                                                QString::fromStdString(usuario->getNombre()),
                                                &ok);
    if (!ok || nuevoNombre.isEmpty()) return;

    QString nuevoApellido = QInputDialog::getText(this, "Modificar Apellido",
                                                  "Nuevo Apellido:",
                                                  QLineEdit::Normal,
                                                  QString::fromStdString(usuario->getApellido()),
                                                  &ok);
    if (!ok || nuevoApellido.isEmpty()) return;

    QString nuevoCorreo = QInputDialog::getText(this, "Modificar Correo",
                                                "Nuevo Correo:",
                                                QLineEdit::Normal,
                                                QString::fromStdString(usuario->getCorreo()),
                                                &ok);
    if (!ok || nuevoCorreo.isEmpty()) return;

    QString nuevaContrasena = QInputDialog::getText(this, "Modificar Contraseña",
                                                    "Nueva Contraseña:",
                                                    QLineEdit::Normal,
                                                    QString::fromStdString(usuario->getContrasena()),
                                                    &ok);
    if (!ok || nuevaContrasena.isEmpty()) return;

    QString nuevaFechaNacimiento = QInputDialog::getText(this, "Modificar Fecha de Nacimiento",
                                                         "Nueva Fecha de Nacimiento:",
                                                         QLineEdit::Normal,
                                                         QString::fromStdString(usuario->getFechaDeNacimiento()),
                                                         &ok);
    if (!ok || nuevaFechaNacimiento.isEmpty()) return;

    // Verificar que el nuevo correo no esté en uso por otro usuario
    if (nuevoCorreo.toStdString() != usuario->getCorreo() && listaUsuarios->usuarioDuplicado(nuevoCorreo.toStdString())) {
        QMessageBox::warning(this, "Error", "El correo electrónico ya está en uso por otro usuario.");
        return;
    }

    // Eliminar el usuario original del árbol (por el correo antiguo)
    listaUsuarios->borrarUsuarioPorCorreo(usuario->getCorreo());

    // Actualizar los datos del usuario
    usuario->setNombre(nuevoNombre.toStdString());
    usuario->setApellido(nuevoApellido.toStdString());
    usuario->setFechaDeNacimiento(nuevaFechaNacimiento.toStdString());
    usuario->setContrasena(nuevaContrasena.toStdString());
    usuario->setCorreo(nuevoCorreo.toStdString());  // Actualizar correo

    // Volver a insertar el usuario en el árbol con los datos actualizados
    listaUsuarios->agregarUsuario(*usuario);

    // Actualizar los datos de la fila en la tabla
    actualizarFilaEnTabla(*usuario, fila);

    // Mostrar mensaje de éxito
    QMessageBox::information(this, "Éxito", "Los datos del usuario se han actualizado correctamente.");
}



void Admin::on_eliminar_usuario_clicked(const std::string& correo)
{

    listaUsuarios->borrarUsuarioPorCorreo(correo);

    // Eliminar la fila correspondiente de la tabla
    for (int i = 0; i < ui->tabla_buscar_admin->rowCount(); ++i) {
        QTableWidgetItem* item = ui->tabla_buscar_admin->item(i, 2);
        if (item && item->text().toStdString() == correo) {
            ui->tabla_buscar_admin->removeRow(i);
            break;
        }
    }
}


void Admin::on_aplicar_orden_comboBox_orden_tabla_usuario_clicked()
{
    // Obtener el texto seleccionado en el combo box
    QString criterioOrden = ui->comboBox_orden_tabla_usuario->currentText();

    // Obtener el vector de usuarios en el orden seleccionado
    std::vector<Usuario> usuariosOrdenados = listaUsuarios->obtenerUsuariosEnOrden(criterioOrden.toStdString());

    // Limpiar la tabla actual antes de actualizarla
    ui->tabla_buscar_admin->setRowCount(0);  // Resetear las filas de la tabla

    // Actualizar la tabla con los usuarios ordenados
    for (const Usuario& usuario : usuariosOrdenados) {
        int row = ui->tabla_buscar_admin->rowCount();  // Obtener el número de filas actuales
        ui->tabla_buscar_admin->insertRow(row);  // Insertar una nueva fila

        // Insertar los datos del usuario en las columnas correspondientes
        ui->tabla_buscar_admin->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(usuario.getNombre())));
        ui->tabla_buscar_admin->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(usuario.getApellido())));
        ui->tabla_buscar_admin->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(usuario.getCorreo())));
        ui->tabla_buscar_admin->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(usuario.getFechaDeNacimiento())));
        // Crear botones de Modificar y Eliminar
        QPushButton* btnModificar = new QPushButton("Modificar");
        QPushButton* btnEliminar = new QPushButton("Eliminar");

        // Añadir los botones a las columnas correspondientes
        ui->tabla_buscar_admin->setCellWidget(row, 4, btnModificar);
        ui->tabla_buscar_admin->setCellWidget(row, 5, btnEliminar);

        // Conectar los botones a sus respectivos slots
        connect(btnModificar, &QPushButton::clicked, [this, usuario, row]() {
            // Pasar el usuario y la fila para poder actualizar
            this->on_modificar_usuario_clicked(usuario.getCorreo(), row);
        });
        connect(btnEliminar, &QPushButton::clicked, [this, usuario]() {
            this->on_eliminar_usuario_clicked(usuario.getCorreo());
        });
    }
}

