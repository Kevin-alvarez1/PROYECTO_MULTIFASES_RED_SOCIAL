#include "usuarios.h"
#include "ui_usuarios.h"
#include "login.h"
#include "QMessageBox"

Usuarios::Usuarios(std::string correoUsuario, ListaUsuarios *listaUsuarios, ListaDoblePublicacion *listadoblepublicacion, ListaSolicitudes *lista_solicitudes, QWidget *parent)
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
        login = new Login(listaUsuarios, listadoblepublicacion, lista_solicitudes, this);
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
            login = new Login(listaUsuarios, listadoblepublicacion, lista_solicitudes, this);
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

void Usuarios::on_btnCancelar_clicked(const std::string& correoReceptor) {

    std::string correoEmisor = correoActualUsuario_;
    lista_solicitudes->eliminarSolicitud(correoEmisor, correoReceptor);
    QMessageBox::information(this, "Solicitud Cancelada", "Se ha cancelado la enviadaa " + QString::fromStdString(correoReceptor) + ".");

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
    lista_solicitudes->enviarSolicitud(correoEmisor, correoReceptor);

    // Mostrar un mensaje de confirmación
    QMessageBox::information(this, "Solicitud Enviada", "Se ha enviado una solicitud a " + QString::fromStdString(correoReceptor) + ".");
}

void Usuarios::on_actualizar_tablas_clicked() {
    QString correoActual = QString::fromStdString(correoActualUsuario_);

    // Obtener usuarios en el orden especificado
    QString criterioOrden = "InOrder";
    std::vector<Usuario> usuarios = listaUsuarios->obtenerUsuariosEnOrden(criterioOrden.toStdString());

    // Obtener la tabla de usuarios
    QTableWidget* tablaUsuarios = findChild<QTableWidget*>("tabla_usuarios_solicitud");
    QTableWidget* tablaSolicitudesEnviadas = findChild<QTableWidget*>("solicitudes_enviadas_tabla");
    QTableWidget* tablaSolicitudesRecibidas = findChild<QTableWidget*>("solicitudes_recibidas_tabla");

    if (tablaUsuarios) {
        std::vector<Usuario> usuariosFiltrados;

        for (const auto& usuario : usuarios) {
            // Evitar mostrar el usuario actual en la tabla
            if (usuario.getCorreo() != correoActual.toStdString()) {
                // Verificar si ya existe una solicitud en estado PENDIENTE o ACEPTADA
                bool solicitudExistente = lista_solicitudes->existeSolicitudEnEstado(
                                              correoActual.toStdString(), usuario.getCorreo(), "PENDIENTE") ||
                                          lista_solicitudes->existeSolicitudEnEstado(
                                              correoActual.toStdString(), usuario.getCorreo(), "ACEPTADA");

                // Si no existe una solicitud en estado PENDIENTE o ACEPTADA, agregar el usuario a la lista filtrada
                if (!solicitudExistente) {
                    usuariosFiltrados.push_back(usuario);
                }
            }
        }

        // Configurar la tabla
        tablaUsuarios->setRowCount(usuariosFiltrados.size());
        tablaUsuarios->setColumnCount(5);

        // Encabezados de la tabla
        tablaUsuarios->setHorizontalHeaderLabels(QStringList() << "Nombre" << "Apellido" << "Correo" << "Fecha de nacimiento" << " ");

        // Rellenar la tabla con los usuarios filtrados
        for (size_t i = 0; i < usuariosFiltrados.size(); ++i) {
            const Usuario& usuario = usuariosFiltrados[i];

            tablaUsuarios->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(usuario.getNombre())));
            tablaUsuarios->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(usuario.getApellido())));
            tablaUsuarios->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(usuario.getCorreo())));
            tablaUsuarios->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(usuario.getFechaDeNacimiento())));

            // Botón para enviar solicitud de amistad
            QPushButton* btnEnviarSolicitud = new QPushButton("Enviar solicitud");
            tablaUsuarios->setCellWidget(i, 4, btnEnviarSolicitud);

            // Conectar el botón a la función de enviar solicitud
            connect(btnEnviarSolicitud, &QPushButton::clicked, [this, usuario]() {
                this->on_btnEnviarSolicitud_clicked(usuario.getCorreo());
            });
        }
    } else {
        qWarning("La tabla de usuarios no se encontró.");
    }

    // ACTUALIZAR la tabla de solicitudes enviadas en estado "PENDIENTE"
    if (tablaSolicitudesEnviadas) {
        // Obtener las solicitudes enviadas en estado "PENDIENTE"
        std::vector<std::string> solicitudesPendientes = lista_solicitudes->obtenerSolicitudesEnviadas(correoActual.toStdString());

        std::vector<std::string> solicitudesValidas;

        // Validar si existe una solicitud "ACEPTADA" en dirección inversa
        for (const std::string& receptor : solicitudesPendientes) {
            bool solicitudAceptadaInversa = lista_solicitudes->existeSolicitudEnEstado(
                receptor, correoActual.toStdString(), "ACEPTADA");

            // Solo agregar la solicitud a la lista si no hay una solicitud ACEPTADA inversa
            if (!solicitudAceptadaInversa) {
                solicitudesValidas.push_back(receptor);
            }
        }

        tablaSolicitudesEnviadas->setRowCount(solicitudesValidas.size());
        tablaSolicitudesEnviadas->setColumnCount(2);

        // Encabezados de la tabla
        tablaSolicitudesEnviadas->setHorizontalHeaderLabels(QStringList() << "Receptor" << "");

        for (size_t i = 0; i < solicitudesValidas.size(); ++i) {
            tablaSolicitudesEnviadas->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(solicitudesValidas[i])));

            QPushButton* btnCancelar = new QPushButton("Cancelar");
            tablaSolicitudesEnviadas->setCellWidget(i, 1, btnCancelar);

            connect(btnCancelar, &QPushButton::clicked, [this, correoReceptor = solicitudesValidas[i]]() {
                this->on_btnCancelar_clicked(correoReceptor);
            });
        }
    } else {
        qWarning("La tabla de solicitudes enviadas no se encontró.");
    }

    if (tablaSolicitudesRecibidas) {
        // Primero, vaciar la pila de solicitudes recibidas
        PilaReceptor& pilaReceptor = obtenerPilaReceptor(correoActual.toStdString());
        while (!pilaReceptor.estaVacia()) {
            pilaReceptor.pop();
        }

        // Luego, obtener y apilar solicitudes pendientes
        lista_solicitudes->buscarYApilarPendientes(correoActual.toStdString(), *lista_solicitudes);

        // Luego, obtener la pila de solicitudes recibidas
        std::vector<Receptor> solicitudesRecibidas;

        // Copia las solicitudes sin vaciar la pila
        PilaReceptor copiaPilaReceptor = pilaReceptor; // Crear una copia de la pila
        while (!copiaPilaReceptor.estaVacia()) {
            solicitudesRecibidas.push_back(copiaPilaReceptor.peek());
            copiaPilaReceptor.pop();
        }

        tablaSolicitudesRecibidas->setRowCount(solicitudesRecibidas.size());
        tablaSolicitudesRecibidas->setColumnCount(3);

        tablaSolicitudesRecibidas->setHorizontalHeaderLabels(QStringList() << "Emisor" << "" << "");

        for (size_t i = 0; i < solicitudesRecibidas.size(); ++i) {
            const Receptor& solicitud = solicitudesRecibidas[i];

            // Verifica si `getEmisor()` es el valor correcto
            tablaSolicitudesRecibidas->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(solicitud.getEmisor())));

            QPushButton* btnAceptar = new QPushButton("Aceptar");
            tablaSolicitudesRecibidas->setCellWidget(i, 1, btnAceptar);

            connect(btnAceptar, &QPushButton::clicked, [this, solicitud]() {
                this->on_btnAceptar_clicked(solicitud.getEmisor());
            });

            QPushButton* btnRechazar = new QPushButton("Rechazar");
            tablaSolicitudesRecibidas->setCellWidget(i, 2, btnRechazar);

            connect(btnRechazar, &QPushButton::clicked, [this, solicitud]() {
                this->on_btnRechazar_clicked(solicitud.getEmisor());
            });
        }
    } else {
        qWarning("La tabla de solicitudes recibidas no se encontró.");
    }

}

void Usuarios::on_btnAceptar_clicked(const std::string& correoEmisor) {
   try {
        std::string correoReceptor = correoActualUsuario_;

    // Verificar si el usuario actual está tratando de enviarse una solicitud a sí mismo
    if (correoEmisor == correoReceptor) {
        QMessageBox::warning(this, "Error", "No puedes enviarte una solicitud a ti mismo.");
        return;
    }

    // Obtener la pila del receptor (usuario actual)
    PilaReceptor& pilaReceptor = obtenerPilaReceptor(correoReceptor);

    // Actualizar el estado de la solicitud en la pila del receptor
    pilaReceptor.actualizarEstadoSolicitud(correoEmisor, correoReceptor, "ACEPTADA");

    // Mostrar un mensaje de confirmación
    QMessageBox::information(this, "Solicitud Aceptada", "La solicitud de " + QString::fromStdString(correoEmisor) + " ha sido aceptada.");
    } catch (const std::exception& e) {
        std::cerr << "Excepción: " << e.what() << std::endl;
    }
}

void Usuarios::on_btnRechazar_clicked(const std::string& correoEmisor) {
    try {
        std::string correoReceptor = correoActualUsuario_;  // Asegúrate de que `correoActualUsuario_` sea de tipo `std::string`

        // Obtener la pila de solicitudes recibidas del usuario actual
        PilaReceptor& pilaReceptor = obtenerPilaReceptor(correoReceptor);

        // Verificar si la pila no está vacía
        if (pilaReceptor.estaVacia()) {
            QMessageBox::warning(this, "Error", "No hay solicitudes para rechazar.");
            return;
        }

        // Intentar encontrar y eliminar la solicitud del emisor
        bool solicitudEliminada = false;
        PilaReceptor pilaTemporal;

        // Procesar la pila para eliminar la solicitud del emisor
        while (!pilaReceptor.estaVacia()) {
            Receptor solicitud = pilaReceptor.peek();
            if (solicitud.getEmisor() == correoEmisor) {
                // Si la solicitud es del emisor que queremos rechazar, no la copiamos a la pila temporal
                pilaReceptor.pop();
                solicitudEliminada = true;
                break;
            } else {
                // Si la solicitud no es la que queremos eliminar, la copiamos a una pila temporal
                pilaTemporal.push(solicitud);
                pilaReceptor.pop();
            }
        }

        // Restaurar las solicitudes que no se eliminaron
        while (!pilaTemporal.estaVacia()) {
            pilaReceptor.push(pilaTemporal.peek());
            pilaTemporal.pop();
        }

        // Eliminar la solicitud de la lista de solicitudes
        lista_solicitudes->eliminarSolicitud(correoEmisor, correoReceptor);

        // Mostrar mensaje de confirmación si la solicitud fue eliminada
        if (solicitudEliminada) {
            QMessageBox::information(this, "Solicitud Rechazada", "La solicitud de " + QString::fromStdString(correoEmisor) + " ha sido rechazada.");
        } else {
            QMessageBox::warning(this, "Error", "No se encontró una solicitud de " + QString::fromStdString(correoEmisor) + ".");
        }

        // Actualizar la interfaz gráfica para reflejar los cambios
        on_actualizar_tablas_clicked();
    } catch (const std::exception& e) {
        std::cerr << "Excepción: " << e.what() << std::endl;
        QMessageBox::critical(this, "Error", "Ocurrió un error al intentar rechazar la solicitud.");
    }
}



