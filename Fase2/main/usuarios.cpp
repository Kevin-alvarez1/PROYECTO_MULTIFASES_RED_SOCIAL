#include "usuarios.h"
#include "ui_usuarios.h"
#include "login.h"
#include "QMessageBox"

Usuarios::Usuarios(std::string correoUsuario, ListaUsuarios *listaUsuarios, ListaDoblePublicacion listadoblepublicacion, ListaSolicitudes lista_solicitudes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Usuarios)
    , login(nullptr)
    , listaUsuarios(listaUsuarios)
    , correoActualUsuario_(correoUsuario)
    , listadoblepublicacion(listadoblepublicacion)
    , lista_solicitudes(lista_solicitudes)
{
    ui->setupUi(this);
    ui->correo_editar_txt->setText(QString::fromStdString(correoUsuario));

}

Usuarios::~Usuarios()
{
    delete ui;
}
void Usuarios::on_cerrar_sesion_btn_clicked()
{
    if (!login) {
        login = new Login(listaUsuarios, this); // Pasar el puntero
    }

    login->show();
    this->hide();
}


// Usuarios.cpp
void Usuarios::on_buscar_correo_btn_clicked()
{
    std::string correo = ui->buscador_usuario_txt->text().toStdString();
    Usuario usuario = listaUsuarios->mostrarDatosPorCorreo(correo);

    if (usuario.getCorreo() == correo)  // Verifica que el usuario devuelto tiene el correo buscado
    {
        ui->buscador_usuario_nombre_txt->setText(QString::fromStdString(usuario.getNombre()));
        ui->buscador_usuario_apellido_txt->setText(QString::fromStdString(usuario.getApellido()));
        ui->buscador_usuario_correo_txt->setText(QString::fromStdString(usuario.getCorreo()));
        ui->buscador_usuario_fecha_nacimiento_txt->setText(QString::fromStdString(usuario.getFechaDeNacimiento()));
    }
    else
    {
        // Manejo de caso cuando el usuario no se encuentra
        ui->buscador_usuario_nombre_txt->clear();
        ui->buscador_usuario_apellido_txt->clear();
        ui->buscador_usuario_correo_txt->clear();
        ui->buscador_usuario_fecha_nacimiento_txt->clear();

        // Muestra un mensaje de advertencia al usuario
        QMessageBox::warning(this, "Error", "Usuario con correo " + QString::fromStdString(correo) + " no encontrado.");
    }
}

void Usuarios::on_Eliminar_boton_clicked()
{
    std::string correo = ui->correo_editar_txt->text().toStdString();

    // Verificar si el correo ingresado es el mismo que el del usuario autenticado
    if (correo != correoActualUsuario_) {
        // Mostrar advertencia y continuar sin cerrar el programa ni la ventana
        QMessageBox::warning(this, "Error", "Solo puedes eliminar tu propia cuenta.");
        return;  // Termina la función para no continuar con la eliminación
    }

    // Confirmar si el usuario realmente quiere borrar su cuenta
    int result = QMessageBox::question(this, "Aviso", "¿Desea borrar su cuenta?", QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        // Si el usuario confirma, eliminamos la cuenta
        listaUsuarios->borrarUsuarioPorCorreo(correo);
        QMessageBox::information(this, "Cuenta Eliminada", "Tu cuenta ha sido eliminada correctamente.");

        // Mostrar la ventana de inicio de sesión después de la eliminación
        if (!login) {
            login = new Login(listaUsuarios, this); // Pasar el puntero
        }
        login->show();
        this->hide();  // Ocultar la ventana actual
    } else {
        // Si el usuario selecciona "No", simplemente cancelamos la operación
        QMessageBox::information(this, "Cancelado", "La eliminación de la cuenta ha sido cancelada.");
    }
}




void Usuarios::on_Modificar_boton_clicked()
{
    // Obtener los nuevos datos desde las áreas de texto
    std::string nuevoNombre = ui->nombre_editar_txt->text().toStdString();
    std::string nuevoApellido = ui->apellido_editar_txt->text().toStdString();
    std::string nuevoCorreo = ui->correo_editar_txt->text().toStdString();
    std::string nuevaContrasena = ui->contrasena_editar_txt->text().toStdString();
    std::string nuevaFechaNacimiento = ui->fecha_nacimiento_edit->text().toStdString();

    // Verificar que los campos no estén vacíos
    if (nuevoNombre.empty() || nuevoApellido.empty() || nuevoCorreo.empty() || nuevaContrasena.empty() || nuevaFechaNacimiento.empty()) {
        QMessageBox::warning(this, "Error", "Por favor, llena todos los campos.");
        return; // No continuar si hay campos vacíos
    }

    // Verificar que el nuevo correo no esté en uso por otro usuario
    if (nuevoCorreo != correoActualUsuario_ && listaUsuarios->usuarioDuplicado(nuevoCorreo)) {
        QMessageBox::warning(this, "Error", "El correo electrónico ya está en uso por otro usuario.");
        return;
    }

    // Buscar al usuario en la lista
    Usuario* usuario = listaUsuarios->buscarUsuarioPorCorreo(correoActualUsuario_);
    if (usuario) {
        // Actualizar los datos del usuario
        usuario->setNombre(nuevoNombre);
        usuario->setApellido(nuevoApellido);
        usuario->setFechaDeNacimiento(nuevaFechaNacimiento);
        usuario->setContrasena(nuevaContrasena);
        usuario->setCorreo(nuevoCorreo); // Actualizar correo

        // Actualizar el correo actual del usuario en la sesión
        correoActualUsuario_ = nuevoCorreo;

        // Mostrar mensaje de éxito
        QMessageBox::information(this, "Éxito", "Los datos del usuario se han actualizado correctamente.");
    } else {
        QMessageBox::warning(this, "Error", "No se pudo encontrar al usuario.");
    }
}


void Usuarios::on_commandLinkButton_clicked() {

    QString criterioOrden = "InOrder";
    QString correoActual = QString::fromStdString(correoActualUsuario_);

    // Obtener usuarios en el orden especificado
    std::vector<Usuario> usuarios = listaUsuarios->obtenerUsuariosEnOrden(criterioOrden.toStdString());

    // Obtener la tabla
    QTableWidget* tabla = findChild<QTableWidget*>("tabla_usuarios_solicitud");

    if (tabla) {
        std::vector<Usuario> usuariosFiltrados;
        for (const auto& usuario : usuarios) {
            if (usuario.getCorreo() != correoActual.toStdString()) {
                usuariosFiltrados.push_back(usuario);
            }
        }

        tabla->setRowCount(usuariosFiltrados.size());
        tabla->setColumnCount(5); // 5 columnas: Nombre, Apellido, Correo, Fecha de nacimiento, Botón

        // Establecer los encabezados de las columnas
        tabla->setHorizontalHeaderLabels(QStringList() << "Nombre" << "Apellido" << "Correo" << "Fecha de nacimiento" << "Acción");

        // Llenar la tabla con los usuarios filtrados
        for (size_t i = 0; i < usuariosFiltrados.size(); ++i) {
            const Usuario& usuario = usuariosFiltrados[i];

            tabla->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(usuario.getNombre())));
            tabla->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(usuario.getApellido())));
            tabla->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(usuario.getCorreo())));
            tabla->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(usuario.getFechaDeNacimiento())));

            // Crear el botón de enviar solicitud
            QPushButton* btnEnviarSolicitud = new QPushButton("Enviar solicitud");

            // Añadir el botón a la tabla en la columna 4
            tabla->setCellWidget(i, 4, btnEnviarSolicitud);

            // Conectar el botón con un slot para manejar el clic, pasando el correo del usuario
            connect(btnEnviarSolicitud, &QPushButton::clicked, [this, usuario]() {
                this->on_btnEnviarSolicitud_clicked(usuario.getCorreo());
            });
        }
    } else {
        qWarning("La tabla no se encontró.");
    }
}


void Usuarios::on_btnEnviarSolicitud_clicked(const std::string& correoReceptor) {
    // Obtener el correo del usuario actual (emisor)
    std::string correoEmisor = correoActualUsuario_;

    // Verificar si el usuario actual está tratando de enviarse una solicitud a sí mismo
    if (correoEmisor == correoReceptor) {
        QMessageBox::warning(this, "Error", "No puedes enviarte una solicitud a ti mismo.");
        return;
    }

    // Añadir la solicitud a la lista de solicitudes (usando la clase `ListaSolicitudes`)
    lista_solicitudes.enviarSolicitud(correoEmisor, correoReceptor);

    // Mostrar un mensaje de confirmación
    QMessageBox::information(this, "Solicitud Enviada", "Se ha enviado una solicitud a " + QString::fromStdString(correoReceptor) + ".");
}



