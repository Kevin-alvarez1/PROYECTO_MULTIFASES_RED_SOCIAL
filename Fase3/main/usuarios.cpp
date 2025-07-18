#include "usuarios.h"
#include "ui_usuarios.h"
#include "login.h"
#include "QMessageBox"
#include "grafo_no_dirigido.h"
#include "arbolabb.h"
#include <QDialog>
#include <QVBoxLayout>
#include <set>
#include "arbolabb.h"
#include <QInputDialog>
#include <unordered_set>
#include <QCryptographicHash>
extern GrafoNoDirigido grafoNoDirigido;
extern ArbolABB arbolABB;
extern ArbolBComentario arbolComentarios_;

Usuarios::Usuarios(std::string correoUsuario, ListaUsuarios *listaUsuarios, ListaDoblePublicacion *listadoblepublicacion, ListaSolicitudes *lista_solicitudes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Usuarios)
    , login(nullptr)
    , listaUsuarios(listaUsuarios)
    , correoActualUsuario_(correoUsuario)
    , listadoblepublicacion(listadoblepublicacion)
    , lista_solicitudes(lista_solicitudes)
    , CrearPublicacion(nullptr)
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

void Usuarios::on_buscar_correo_btn_clicked()
{
    std::string correo = ui->buscador_usuario_txt->text().toStdString();
    Usuario usuario = listaUsuarios->mostrarDatosPorCorreo(correo);

    if (usuario.getCorreo() == correo)
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

    if (correo != correoActualUsuario_) {
        QMessageBox::warning(this, "Error", "Solo puedes eliminar tu propia cuenta.");
        return;
    }

    // Confirmar si el usuario realmente quiere borrar su cuenta
    int result = QMessageBox::question(this, "Aviso", "¿Desea borrar su cuenta?", QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        // Si el usuario confirma, eliminamos la cuenta
        listaUsuarios->borrarUsuarioPorCorreo(correo);
        listadoblepublicacion->eliminarPublicacionesPorCorreo(correo);
        arbolComentarios_.eliminarComentariosPorCorreo(correo);
        grafoNoDirigido.eliminarUsuario(correo);

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
        // Encriptar la nueva contraseña
        QByteArray hash = QCryptographicHash::hash(QByteArray::fromStdString(nuevaContrasena), QCryptographicHash::Sha256);
        std::string nuevaContrasenaEncriptada = hash.toHex().constData();

        // Actualizar los datos del usuario
        usuario->setNombre(nuevoNombre);
        usuario->setApellido(nuevoApellido);
        usuario->setFechaDeNacimiento(nuevaFechaNacimiento);
        usuario->setContrasena(nuevaContrasenaEncriptada); // Actualizar contraseña encriptada
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

        // Obtener la lista de amigos del usuario actual
        int cantidadAmigos = 0;
        std::string* amigos = grafoNoDirigido.obtenerAmigos(correoActual.toStdString(), cantidadAmigos);

        // Crear un conjunto para almacenar los amigos
        std::unordered_set<std::string> amigosSet(amigos, amigos + cantidadAmigos);

        for (const auto& usuario : usuarios) {
            // Evitar mostrar el usuario actual en la tabla
            if (usuario.getCorreo() != correoActual.toStdString()) {
                // Verificar si el usuario ya es amigo
                if (amigosSet.find(usuario.getCorreo()) == amigosSet.end()) {
                    // Si no es amigo, agregarlo a la lista filtrada
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

        // Liberar la memoria si amigos fue alocado dinámicamente
        delete[] amigos;  // Asumiendo que `obtenerAmigos` devuelve memoria dinámica
    }

    // ACTUALIZAR la tabla de solicitudes enviadas en estado "PENDIENTE"
    if (tablaSolicitudesEnviadas) {
        // Obtener las solicitudes enviadas en estado "PENDIENTE"
        std::vector<std::string> solicitudesPendientes = lista_solicitudes->obtenerSolicitudesEnviadas(correoActual.toStdString());

        std::vector<std::string> solicitudesValidas;

        // Validar si existe una solicitud "ACEPTADA" en cualquier dirección
        for (const std::string& receptor : solicitudesPendientes) {
            bool solicitudAceptadaDirecta = lista_solicitudes->existeSolicitudEnEstado(
                correoActual.toStdString(), receptor, "ACEPTADA");
            bool solicitudAceptadaInversa = lista_solicitudes->existeSolicitudEnEstado(
                receptor, correoActual.toStdString(), "ACEPTADA");

            // Solo agregar la solicitud a la lista si no hay una solicitud ACEPTADA en ninguna dirección
            if (!solicitudAceptadaDirecta && !solicitudAceptadaInversa) {
                solicitudesValidas.push_back(receptor);
            }
        }

        // Actualizar la tabla con las solicitudes válidas
        tablaSolicitudesEnviadas->setRowCount(solicitudesValidas.size());
        tablaSolicitudesEnviadas->setColumnCount(2);

        // Encabezados de la tabla
        tablaSolicitudesEnviadas->setHorizontalHeaderLabels(QStringList() << "Receptor" << "");

        for (size_t i = 0; i < solicitudesValidas.size(); ++i) {
            tablaSolicitudesEnviadas->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(solicitudesValidas[i])));

            // Botón para cancelar la solicitud
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
            Receptor solicitud = copiaPilaReceptor.peek();

            // Verifica si la solicitud está en estado "PENDIENTE"
            if (lista_solicitudes->existeSolicitudEnEstado(solicitud.getEmisor(), correoActual.toStdString(), "PENDIENTE")) {

                int cantidadAmigos;
                std::string* amigos = grafoNoDirigido.obtenerAmigos(correoActual.toStdString(), cantidadAmigos);

                // Verificar si el emisor ya es amigo del usuario actual
                bool esAmigo = false;
                for (int i = 0; i < cantidadAmigos; i++) {
                    if (amigos[i] == solicitud.getEmisor()) {
                        esAmigo = true;
                        break; // Salir del bucle si ya son amigos
                    }
                }

                // Si no son amigos, agregar la solicitud a la lista para mostrarla
                if (!esAmigo) {
                    solicitudesRecibidas.push_back(solicitud);
                }

                // Liberar la memoria del arreglo de amigos
                delete[] amigos; // Asegúrate de liberar la memoria después de su uso
            }
            copiaPilaReceptor.pop();
        }

        // Ahora que tenemos la lista de solicitudes no duplicadas, construimos la tabla
        tablaSolicitudesRecibidas->setRowCount(solicitudesRecibidas.size());
        tablaSolicitudesRecibidas->setColumnCount(3);

        tablaSolicitudesRecibidas->setHorizontalHeaderLabels(QStringList() << "Emisor" << "" << "");

        for (size_t i = 0; i < solicitudesRecibidas.size(); ++i) {
            const Receptor& solicitud = solicitudesRecibidas[i];

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

        // Verificar si el usuario actual está tratando de aceptarse una solicitud a sí mismo
        if (correoEmisor == correoReceptor) {
            QMessageBox::warning(this, "Error", "No puedes aceptar una solicitud de ti mismo.");
            return;
        }

        // Obtener la pila del receptor (usuario actual)
        PilaReceptor& pilaReceptor = obtenerPilaReceptor(correoReceptor);

        // Recorrer la pila del receptor manualmente
        NodoReceptor* actual = pilaReceptor.getCima();  // Obtener la cima de la pila
        NodoReceptor* anterior = nullptr;

        while (actual) {
            if (actual->receptor.getEmisor() == correoEmisor) {
                // Eliminar de la pila
                if (anterior) {
                    anterior->siguiente = actual->siguiente;
                } else {
                    pilaReceptor.setCima(actual->siguiente);  // Actualizar la cima de la pila
                }

                // Actualizar el estado de la solicitud en lista_solicitudes
                if (lista_solicitudes->actualizarEstadoSolicitud(correoEmisor, correoReceptor, "ACEPTADA")) {
                    // Insertar en la matriz dispersa la nueva relación de amistad
                    grafoNoDirigido.insertarRelacion(correoEmisor, correoReceptor);

                    // Mostrar un mensaje de confirmación
                    QMessageBox::information(this, "Solicitud Aceptada",
                                             "La solicitud de " + QString::fromStdString(correoEmisor) + " ha sido aceptada.");

                    grafoNoDirigido.mostrarGrafo();
                    // Eliminar el nodo de la memoria
                    delete actual;
                    return;
                } else {
                    // Error en la actualización de lista_solicitudes
                    QMessageBox::warning(this, "Error", "No se pudo actualizar la solicitud en lista_solicitudes.");
                }
            }
            anterior = actual;
            actual = actual->siguiente;
        }

        // Si no se encuentra la solicitud
        QMessageBox::warning(this, "Error",
                             "No se encontró una solicitud de amistad del emisor " + QString::fromStdString(correoEmisor) + ".");
    } catch (const std::exception& e) {
        std::cerr << "Excepción: " << e.what() << std::endl;
        QMessageBox::critical(this, "Error", "Ocurrió un error al aceptar la solicitud.");
    }
}

void Usuarios::on_btnRechazar_clicked(const std::string& correoEmisor) {
    try {
        std::string correoReceptor = correoActualUsuario_;

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




void Usuarios::on_crear_nueva_publi_boton_clicked()
{
    if (!CrearPublicacion) {
        CrearPublicacion = new class CrearPublicacion(correoActualUsuario_, listadoblepublicacion, this);
    }

    CrearPublicacion->show();
    this->hide();
}

void Usuarios::on_fecha_filtro_publis_boton_clicked()
{
    QString fechaSeleccionada = ui->fechas_filtro_publicaciones_comboBox->currentText();

    // Aquí puedes llamar al método para mostrar las publicaciones filtradas
    mostrarPublicacionesFiltradasPorFecha(fechaSeleccionada);
}

void Usuarios::mostrarPublicacionesFiltradasPorFecha(const QString& fechaSeleccionada)
{
    try {
        std::string correoUsuario = correoActualUsuario_;
        std::cout << "Publicaciones del usuario " << correoUsuario << ":" << std::endl;

        QWidget* publicacionesFrame = ui->scrollAreaWidgetContents;

        // Limpiar el layout de publicaciones anteriores
        QLayout* layout = publicacionesFrame->layout();
        if (layout != nullptr) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
        } else {
            layout = new QVBoxLayout(publicacionesFrame);
            publicacionesFrame->setLayout(layout);
        }

        layout->setSpacing(10);
        layout->setContentsMargins(10, 10, 10, 10);
        publicacionesFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Obtener todas las publicaciones y amigos
        std::vector<Publicacion> publicaciones_arbol = listadoblepublicacion->mostrarPublicacionesYAmigos(correoUsuario, grafoNoDirigido, arbolABB, "InOrder");
        std::cout << "Total de publicaciones encontradas: " << publicaciones_arbol.size() << std::endl;
        // Llenar el ComboBox con las fechas disponibles en el BST
        llenarComboBoxFechas_BST();
        std::vector<Publicacion> publicacionesFiltradas;

        // Filtrar publicaciones por fecha seleccionada
        if (fechaSeleccionada != "Todos") {
            for (const auto& publicacion : publicaciones_arbol) {
                if (QString::fromStdString(publicacion.getFecha()) == fechaSeleccionada) {
                    publicacionesFiltradas.push_back(publicacion);
                }
            }
        } else {
            publicacionesFiltradas = publicaciones_arbol; // Si se selecciona "Todos"
        }

        // Mostrar publicaciones filtradas
        int cantidadAMostrar = ui->cantidad_a_mostrar_publis_txt->text().toInt();
        int publicacionesAmostrar = std::min(cantidadAMostrar, static_cast<int>(publicacionesFiltradas.size()));

        for (int i = 0; i < publicacionesAmostrar; ++i) {
            const auto& publicacion = publicacionesFiltradas[i];

            QFrame* panel = new QFrame(publicacionesFrame);
            panel->setFrameShape(QFrame::StyledPanel);
            panel->setFrameShadow(QFrame::Raised);
            panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

            QVBoxLayout* panelLayout = new QVBoxLayout(panel);
            panel->setLayout(panelLayout);

            QLabel* correoLabel = new QLabel(QString("Correo: %1").arg(QString::fromStdString(publicacion.getCorreo())), panel);
            panelLayout->addWidget(correoLabel);

            QLabel* fechaLabel = new QLabel(QString("Fecha: %1").arg(QString::fromStdString(publicacion.getFecha())), panel);
            panelLayout->addWidget(fechaLabel);

            QLabel* contenidoLabel = new QLabel(QString("Contenido: %1").arg(QString::fromStdString(publicacion.getContenido())), panel);
            panelLayout->addWidget(contenidoLabel);

            QLabel* imagenLabel = new QLabel(panel);
            imagenLabel->setFixedSize(500, 181);
            imagenLabel->setScaledContents(true);

            QString baseRutaImagen = QString("C:\\Users\\Cito\\Desktop\\carpeta EDD\\-EDD-Proyecto1_202203038\\Fase2\\main\\build\\Desktop_Qt_6_7_2_MinGW_64_bit-Profile\\Imagenes\\") + QString::fromStdString(std::to_string(publicacion.getId()));
            QStringList extensiones = QStringList() << ".png" << ".jpg" << ".jpeg" << ".bmp" << ".gif";

            bool imagenCargada = false;

            for (const QString& ext : extensiones) {
                QString rutaImagen = baseRutaImagen + ext;
                QPixmap pixmap(rutaImagen);
                if (!pixmap.isNull()) {
                    QPixmap scaledPixmap = pixmap.scaled(imagenLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    imagenLabel->setPixmap(scaledPixmap);
                    imagenCargada = true;
                    break;
                }
            }

            if (!imagenCargada) {
                // Placeholder visible
                QPixmap placeholder("");
                QPixmap scaledPlaceholder = placeholder.scaled(imagenLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                imagenLabel->setPixmap(scaledPlaceholder);
            }

            panelLayout->addWidget(imagenLabel, 0, Qt::AlignCenter);
            panelLayout->addSpacing(80);

            // Layout para los botones
            QHBoxLayout* botonesLayout = new QHBoxLayout();
            panelLayout->addLayout(botonesLayout);

            QPushButton* comentarButton = new QPushButton("Comentar", panel);
            botonesLayout->addWidget(comentarButton);
            connect(comentarButton, &QPushButton::clicked, this, [this, publicacion] {
                // Solicitar al usuario que ingrese un comentario
                bool ok;
                QString textoComentario = QInputDialog::getText(this, tr("Comentar Publicación"),
                                                                tr("Escribe tu comentario:"), QLineEdit::Normal,
                                                                "", &ok);

                // Verificar si el usuario hizo clic en "Aceptar"
                if (ok && !textoComentario.isEmpty()) {
                    // Solicitar al usuario que ingrese la fecha en formato día/mes/año
                    QString comentarioFecha = QInputDialog::getText(this, tr("Fecha del Comentario"),
                                                                    tr("Ingresa la fecha (dd/mm/yyyy):"), QLineEdit::Normal,
                                                                    "", &ok);

                    // Verificar que la fecha se haya ingresado correctamente
                    if (!ok || comentarioFecha.isEmpty()) {
                        QMessageBox::warning(this, tr("Fecha Vacía"), tr("No puedes enviar un comentario sin fecha."));
                        return; // Salir si no se ingresó una fecha
                    }

                    // Solicitar al usuario que ingrese la hora
                    QString comentarioHora = QInputDialog::getText(this, tr("Hora del Comentario"),
                                                                   tr("Ingresa la hora (HH:mm:ss):"), QLineEdit::Normal,
                                                                   "", &ok);

                    // Verificar que la hora se haya ingresado correctamente
                    if (!ok || comentarioHora.isEmpty()) {
                        QMessageBox::warning(this, tr("Hora Vacía"), tr("No puedes enviar un comentario sin hora."));
                        return; // Salir si no se ingresó una hora
                    }

                    // Convertir std::string a QString si es necesario
                    QString comentarioCorreo = QString::fromStdString(correoActualUsuario_); // Correo del usuario actual

                    // Crear un nuevo comentario
                    Comentario nuevoComentario(textoComentario.toStdString(), publicacion.getId(),
                                               comentarioCorreo.toStdString(),
                                               comentarioFecha.toStdString(),
                                               comentarioHora.toStdString());

                    // Lógica para agregar el comentario al árbol de comentarios
                    arbolComentarios_.insertar(nuevoComentario);

                    // Informar al usuario si el comentario fue agregado correctamente
                    QMessageBox::information(this, tr("Comentario Agregado"),
                                             tr("Tu comentario ha sido agregado exitosamente."));
                } else if (ok) {
                    QMessageBox::warning(this, tr("Comentario Vacío"),
                                         tr("No puedes enviar un comentario vacío."));
                }
            });

            QPushButton* verComentariosButton = new QPushButton("Ver Comentarios", panel);
            botonesLayout->addWidget(verComentariosButton);
            connect(verComentariosButton, &QPushButton::clicked, this, [this, publicacion] {
                mostrarComentariosDePublicacion(publicacion);
            });

            QPushButton* verArbolComentariosButton = new QPushButton("Ver Árbol de Comentarios", panel);
            botonesLayout->addWidget(verArbolComentariosButton);
            connect(verArbolComentariosButton, &QPushButton::clicked, this, [this, publicacion] {
                // Generar el archivo DOT para el árbol de comentarios
                std::string filename = "arbol_comentarios";
                arbolComentarios_.graficarArbolB(filename, publicacion.getId());

                // Crear el comando para ejecutar Graphviz y generar el gráfico
                std::string command = "dot -Tpng " + filename + " -o arbol_comentarios.png";
                system(command.c_str()); // Ejecutar el comando en el sistema

                // Crear e mostrar la ventana para el árbol de comentarios
                VentanaArbolComentarios* ventana = new VentanaArbolComentarios(this);
                ventana->exec(); // Usar exec() para abrirla como un diálogo modal
            });


            botonesLayout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

            QPushButton* eliminarButton = new QPushButton("🗑", panel);
            botonesLayout->addWidget(eliminarButton);
            connect(eliminarButton, &QPushButton::clicked, this, [this, publicacion] {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Confirmar Eliminación", "¿Estás seguro de que deseas eliminar esta publicación?",
                                              QMessageBox::Yes | QMessageBox::No);

                // Verificar si el usuario actual es el dueño de la publicación
                if (publicacion.getCorreo() != correoActualUsuario_) {
                    QMessageBox::warning(this, "Error de Eliminación", "No tienes permiso para eliminar esta publicación.");
                    return; // Salir de la función si el usuario no es el dueño
                }

                if (reply == QMessageBox::Yes) {
                    arbolABB.eliminarPublicacion(publicacion.getId());
                    listadoblepublicacion->eliminarPublicacionPorId(publicacion.getId());
                    // Informar al usuario sobre la eliminación
                    QMessageBox::information(this, "Publicación Eliminada", "La publicación ha sido eliminada exitosamente.");
                }
            });


            layout->addWidget(panel);
        }

        publicacionesFrame->setMinimumHeight(layout->count() * 500);
        ui->scrollArea_2->update();
        ui->scrollArea_2->repaint();

    } catch (const std::exception& e) {
        std::cerr << "Error al mostrar publicaciones: " << e.what() << std::endl;
        QMessageBox::critical(this, "Error", "Hubo un problema al mostrar las publicaciones.");
    }
}


void Usuarios::on_aplicar_orden_publis_boton_clicked()
{
    try {
        QString criterioOrdenQt = ui->ordenPublicaciones_comboBox->currentText();
        std::string criterioOrden = criterioOrdenQt.toUtf8().constData();

        int cantidadAMostrar = ui->cantidad_a_mostrar_publis_txt->text().toInt();
        if (cantidadAMostrar <= 0) {
            QMessageBox::warning(this, "Advertencia", "La cantidad debe ser un número positivo.");
            return;
        }

        if (!listadoblepublicacion) {
            QMessageBox::critical(this, "Error", "ListaDoblePublicacion no está inicializado.");
            return;
        }

        std::string correoUsuario = correoActualUsuario_;
        std::cout << "Publicaciones del usuario " << correoUsuario << ":" << std::endl;

        QWidget* publicacionesFrame = ui->scrollAreaWidgetContents;

        // Limpiar el layout de publicaciones anteriores
        QLayout* layout = publicacionesFrame->layout();
        if (layout != nullptr) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
        } else {
            layout = new QVBoxLayout(publicacionesFrame);
            publicacionesFrame->setLayout(layout);
        }

        layout->setSpacing(10);
        layout->setContentsMargins(10, 10, 10, 10);
        publicacionesFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        std::vector<Publicacion> publicaciones_arbol = listadoblepublicacion->mostrarPublicacionesYAmigos(correoUsuario, grafoNoDirigido, arbolABB, criterioOrden);
        std::cout << "Total de publicaciones encontradas: " << publicaciones_arbol.size() << std::endl;

        connect(ui->fecha_filtro_publis_boton, &QPushButton::clicked, this, &Usuarios::llenarComboBoxFechas);
        llenarComboBoxFechas_BST();
        QString fechaSeleccionada = ui->fechas_filtro_publicaciones_comboBox->currentText();
        std::vector<Publicacion> publicacionesFiltradas;

        if (fechaSeleccionada == "Todos") {
            publicacionesFiltradas = publicaciones_arbol;
        } else {
            for (const auto& publicacion : publicaciones_arbol) {
                if (QString::fromStdString(publicacion.getFecha()) == fechaSeleccionada) {
                    publicacionesFiltradas.push_back(publicacion);
                }
            }
        }


        int publicacionesAmostrar = std::min(cantidadAMostrar, static_cast<int>(publicaciones_arbol.size()));

        for (int i = 0; i < publicacionesAmostrar; ++i) {
            const auto& publicacion = publicaciones_arbol[i];

            QFrame* panel = new QFrame(publicacionesFrame);
            panel->setFrameShape(QFrame::StyledPanel);
            panel->setFrameShadow(QFrame::Raised);
            panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

            QVBoxLayout* panelLayout = new QVBoxLayout(panel);
            panel->setLayout(panelLayout);

            QLabel* correoLabel = new QLabel(QString("Correo: %1").arg(QString::fromStdString(publicacion.getCorreo())), panel);
            panelLayout->addWidget(correoLabel);

            QLabel* fechaLabel = new QLabel(QString("Fecha: %1").arg(QString::fromStdString(publicacion.getFecha())), panel);
            panelLayout->addWidget(fechaLabel);

            QLabel* contenidoLabel = new QLabel(QString("Contenido: %1").arg(QString::fromStdString(publicacion.getContenido())), panel);
            panelLayout->addWidget(contenidoLabel);

            QLabel* imagenLabel = new QLabel(panel);
            imagenLabel->setFixedSize(500, 181);
            imagenLabel->setScaledContents(true);

            QString baseRutaImagen = QString("C:\\Users\\Cito\\Desktop\\carpeta EDD\\-EDD-Proyecto1_202203038\\Fase2\\main\\build\\Desktop_Qt_6_7_2_MinGW_64_bit-Profile\\Imagenes\\") + QString::fromStdString(std::to_string(publicacion.getId()));
            QStringList extensiones = QStringList() << ".png" << ".jpg" << ".jpeg" << ".bmp" << ".gif";

            bool imagenCargada = false;

            for (const QString& ext : extensiones) {
                QString rutaImagen = baseRutaImagen + ext;
                std::cout << "ruta: " << rutaImagen.toStdString() << std::endl;
                QPixmap pixmap(rutaImagen);
                if (!pixmap.isNull()) {
                    QPixmap scaledPixmap = pixmap.scaled(imagenLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    imagenLabel->setPixmap(scaledPixmap);
                    imagenCargada = true;
                    break;
                }
            }

            if (!imagenCargada) {
                // Placeholder visible
                QPixmap placeholder("");
                QPixmap scaledPlaceholder = placeholder.scaled(imagenLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                imagenLabel->setPixmap(scaledPlaceholder);
            }

            panelLayout->addWidget(imagenLabel, 0, Qt::AlignCenter);
            panelLayout->addSpacing(80);

            // Layout para los botones
            QHBoxLayout* botonesLayout = new QHBoxLayout();
            panelLayout->addLayout(botonesLayout);

            QPushButton* comentarButton = new QPushButton("Comentar", panel);
            botonesLayout->addWidget(comentarButton);
            // Conectar a slot
            connect(comentarButton, &QPushButton::clicked, this, [this, publicacion] {
                // Solicitar al usuario que ingrese un comentario
                bool ok;
                QString textoComentario = QInputDialog::getText(this, tr("Comentar Publicación"),
                                                                tr("Escribe tu comentario:"), QLineEdit::Normal,
                                                                "", &ok);

                // Verificar si el usuario hizo clic en "Aceptar"
                if (ok && !textoComentario.isEmpty()) {
                    // Solicitar al usuario que ingrese la fecha en formato día/mes/año
                    QString comentarioFecha = QInputDialog::getText(this, tr("Fecha del Comentario"),
                                                                    tr("Ingresa la fecha (dd/mm/yyyy):"), QLineEdit::Normal,
                                                                    "", &ok);

                    // Verificar que la fecha se haya ingresado correctamente
                    if (!ok || comentarioFecha.isEmpty()) {
                        QMessageBox::warning(this, tr("Fecha Vacía"), tr("No puedes enviar un comentario sin fecha."));
                        return; // Salir si no se ingresó una fecha
                    }

                    // Solicitar al usuario que ingrese la hora
                    QString comentarioHora = QInputDialog::getText(this, tr("Hora del Comentario"),
                                                                   tr("Ingresa la hora (HH:mm:ss):"), QLineEdit::Normal,
                                                                   "", &ok);

                    // Verificar que la hora se haya ingresado correctamente
                    if (!ok || comentarioHora.isEmpty()) {
                        QMessageBox::warning(this, tr("Hora Vacía"), tr("No puedes enviar un comentario sin hora."));
                        return; // Salir si no se ingresó una hora
                    }

                    // Convertir std::string a QString si es necesario
                    QString comentarioCorreo = QString::fromStdString(correoActualUsuario_); //

                    // Crear un nuevo comentario
                    Comentario nuevoComentario(textoComentario.toStdString(), publicacion.getId(),
                                               comentarioCorreo.toStdString(),
                                               comentarioFecha.toStdString(),
                                               comentarioHora.toStdString());

                    // Lógica para agregar el comentario al árbol de comentarios
                    arbolComentarios_.insertar(nuevoComentario);

                    // Informar al usuario si el comentario fue agregado correctamente
                    QMessageBox::information(this, tr("Comentario Agregado"),
                                             tr("Tu comentario ha sido agregado exitosamente."));
                } else if (ok) {
                    QMessageBox::warning(this, tr("Comentario Vacío"),
                                         tr("No puedes enviar un comentario vacío."));
                }
            });

            QPushButton* verComentariosButton = new QPushButton("Ver Comentarios", panel);
            botonesLayout->addWidget(verComentariosButton);
            // Conectar a slot
            connect(verComentariosButton, &QPushButton::clicked, this, [this, publicacion] {
                mostrarComentariosDePublicacion(publicacion);
            });


            QPushButton* verArbolComentariosButton = new QPushButton("Ver Árbol de Comentarios", panel);
            botonesLayout->addWidget(verArbolComentariosButton);
            // Conectar a slot
            connect(verArbolComentariosButton, &QPushButton::clicked, this, [this, publicacion] {
                // Generar el archivo DOT para el árbol de comentarios
                std::string filename = "arbol_comentarios";
                arbolComentarios_.graficarArbolB(filename, publicacion.getId());

                // Crear el comando para ejecutar Graphviz y generar el gráfico
                std::string command = "dot -Tpng " + filename + " -o arbol_comentarios.png";
                system(command.c_str()); // Ejecutar el comando en el sistema

                // Crear e mostrar la ventana para el árbol de comentarios
                VentanaArbolComentarios* ventana = new VentanaArbolComentarios(this);
                ventana->exec(); // Usar exec() para abrirla como un diálogo modal
            });

            botonesLayout->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

            QPushButton* eliminarButton = new QPushButton("🗑", panel);
            botonesLayout->addWidget(eliminarButton);
            // Conectar a slot
            connect(eliminarButton, &QPushButton::clicked, this, [this, publicacion] {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Confirmar Eliminación", "¿Estás seguro de que deseas eliminar esta publicación?",
                                              QMessageBox::Yes | QMessageBox::No);

                // Verificar si el usuario actual es el dueño de la publicación
                if (publicacion.getCorreo() != correoActualUsuario_) {
                    QMessageBox::warning(this, "Error de Eliminación", "No tienes permiso para eliminar esta publicación.");
                    return; // Salir de la función si el usuario no es el dueño
                }

                if (reply == QMessageBox::Yes) {
                    arbolABB.eliminarPublicacion(publicacion.getId());
                    listadoblepublicacion->eliminarPublicacionPorId(publicacion.getId());
                    // Informar al usuario sobre la eliminación
                    QMessageBox::information(this, "Publicación Eliminada", "La publicación ha sido eliminada exitosamente.");
                }
            });

            layout->addWidget(panel);
        }

        publicacionesFrame->setMinimumHeight(layout->count() * 500);
        ui->scrollArea_2->update();
        ui->scrollArea_2->repaint();

    } catch (const std::exception& e) {
        std::cerr << "Error al mostrar publicaciones: " << e.what() << std::endl;
        QMessageBox::critical(this, "Error", "Hubo un problema al mostrar las publicaciones.");
        return;
    }

    QMessageBox::information(this, "Publicaciones", "Se han mostrado todas las publicaciones del usuario.");
}


void Usuarios::mostrarComentariosDePublicacion(const Publicacion& publicacion) {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Comentarios de la publicación");
    QVBoxLayout* dialogLayout = new QVBoxLayout(dialog);

    std::cout << "Publicacion: " << publicacion.getId() << std::endl;

    // Extraer los comentarios asociados a la publicación
    std::vector<Comentario> comentarios = arbolComentarios_.getComentariosDePublicacion(publicacion.getId());

    // Debug: Imprimir cantidad de comentarios
    std::cout << "Comentarios encontrados para publicación ID: " << publicacion.getId() << ": " << comentarios.size() << std::endl;

    if (comentarios.empty()) {
        QLabel* noCommentsLabel = new QLabel("No hay comentarios para esta publicación.", dialog);
        dialogLayout->addWidget(noCommentsLabel);
    } else {
        // Mostrar los comentarios
        for (const auto& comentario : comentarios) {
            QLabel* comentarioLabel = new QLabel(QString("Comentario: %1").arg(QString::fromStdString(comentario.getComentario())), dialog);
            dialogLayout->addWidget(comentarioLabel);

            QLabel* autorLabel = new QLabel(QString("Autor: %1").arg(QString::fromStdString(comentario.getCorreo())), dialog);
            dialogLayout->addWidget(autorLabel);

            QLabel* fechaLabel = new QLabel(QString("Fecha: %1 %2").arg(QString::fromStdString(comentario.getFecha()), QString::fromStdString(comentario.getHora())), dialog);
            dialogLayout->addWidget(fechaLabel);

            dialogLayout->addSpacing(10);  // Separar los comentarios
        }
    }

    QPushButton* cerrarButton = new QPushButton("Cerrar", dialog);
    dialogLayout->addWidget(cerrarButton);
    connect(cerrarButton, &QPushButton::clicked, dialog, &QDialog::accept);

    dialog->setLayout(dialogLayout);
    dialog->exec();

    // Limpieza de recursos
    delete dialog;  // Eliminar el diálogo después de cerrarlo
}


void Usuarios::llenarComboBoxFechas()
{
    std::set<QString> fechasUnicas;
    std::vector<Publicacion> publicaciones = listadoblepublicacion->mostrarPublicacionesYAmigos(correoActualUsuario_, grafoNoDirigido, arbolABB, "InOrder");

    for (const auto& publicacion : publicaciones) {
        fechasUnicas.insert(QString::fromStdString(publicacion.getFecha()));
    }
    fechasUnicas.insert("Todos");

    ui->fechas_filtro_publicaciones_comboBox->clear();
    for (const auto& fecha : fechasUnicas) {
        ui->fechas_filtro_publicaciones_comboBox->addItem(fecha);
    }
}

void Usuarios::llenarComboBoxFechas_BST()
{
    std::set<QString> fechasUnicas;
    std::vector<Publicacion> publicaciones = listadoblepublicacion->mostrarPublicacionesYAmigos(correoActualUsuario_, grafoNoDirigido, arbolABB, "InOrder");

    for (const auto& publicacion : publicaciones) {
        // Obtener la fecha de la publicación y convertirla al formato año/mes/día
        std::string fechaOriginal = publicacion.getFecha();

        // Suponiendo que la fecha original está en formato "día/mes/año", lo convertimos a "año/mes/día"
        QString fechaFormateada;
        QStringList partesFecha = QString::fromStdString(fechaOriginal).split('/');

        if (partesFecha.size() == 3) {
            QString dia = partesFecha[0];
            QString mes = partesFecha[1];
            QString anio = partesFecha[2];

            // Convertimos al formato año/mes/día
            fechaFormateada = anio + "/" + mes + "/" + dia;
        } else {
            // Si el formato no es válido, usamos la fecha original sin cambios
            fechaFormateada = QString::fromStdString(fechaOriginal);
        }

        // Insertamos la fecha formateada en el set de fechas únicas
        fechasUnicas.insert(fechaFormateada);
    }

    // Agregamos la opción para "Todos"
    fechasUnicas.insert("Todos");

    // Limpiar el ComboBox antes de llenarlo
    ui->bst_mostrar_publi_comboBox->clear();

    // Agregar las fechas al ComboBox
    for (const auto& fecha : fechasUnicas) {
        ui->bst_mostrar_publi_comboBox->addItem(fecha);
    }
}

void Usuarios::on_generar_bst_reporte_boton_clicked() {


    // Obtener la fecha seleccionada del ComboBox
    QString fechaSeleccionada = ui->bst_mostrar_publi_comboBox->currentText();

    if (fechaSeleccionada.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se ha seleccionado ninguna fecha.");
        return;
    }

    // Convertir la fecha seleccionada a std::string
    std::string fecha = fechaSeleccionada.toStdString();

    // Generar el archivo DOT para el árbol, basado en la fecha seleccionada
    arbolABB.generateDotFile("ArbolBB", fecha);

    // Convertir el archivo DOT a una imagen PNG usando el comando dot
    std::string comando = "dot -Tpng ArbolBB.dot -o ArbolBB.png";
    int resultado = system(comando.c_str());

    if (resultado != 0) {
        // Manejar el error en caso de que la conversión falle
        QMessageBox::warning(this, "Error", "No se pudo generar la imagen del árbol.");
        return;
    }

    // Actualizar el panel con la nueva imagen
    actualizarPanelConImagen(QString::fromStdString("ArbolBB.png"));
}


void Usuarios::actualizarPanelConImagen(const QString& imagePath) {
    QLayout* existingLayout = ui->arbolBB_frame->layout();
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            delete item->widget();
            delete item;
        }
        delete existingLayout;
    }

    QVBoxLayout* newLayout = new QVBoxLayout();
    ui->arbolBB_frame->setLayout(newLayout);

    QLabel* imageLabel = new QLabel();
    QPixmap pixmap(imagePath);
    imageLabel->setPixmap(pixmap);

    imageLabel->resize(pixmap.size());

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(imageLabel);

    newLayout->addWidget(scrollArea);
}

void Usuarios::on_generar_reportes_usuario_boton_clicked()
{

    // Limpiar la tabla antes de agregar nuevos datos
    ui->fechas_con_mas_publis_tabla->clearContents();
    ui->fechas_con_mas_publis_tabla->setRowCount(0);

    // Crear una lista para almacenar las fechas y sus respectivas cantidades de publicaciones
    std::vector<std::pair<std::string, int>> fechasCantidad;

    // Llenar el vector con las fechas y la cantidad de publicaciones
    llenarFechasCantidad(arbolABB.getRaiz(), fechasCantidad);

    // Ordenar las fechas por la cantidad de publicaciones en orden descendente
    std::sort(fechasCantidad.begin(), fechasCantidad.end(), [](const auto& a, const auto& b) {
        return b.second < a.second;
    });

    // Llenar la tabla con las fechas y la cantidad de publicaciones (Top 3)
    for (size_t i = 0; i < std::min(fechasCantidad.size(), size_t(3)); ++i) {
        ui->fechas_con_mas_publis_tabla->insertRow(i);

        // Insertar la fecha en la primera columna
        QTableWidgetItem* fechaItem = new QTableWidgetItem(QString::fromStdString(fechasCantidad[i].first));
        ui->fechas_con_mas_publis_tabla->setItem(i, 0, fechaItem);

        // Insertar la cantidad de publicaciones en la segunda columna
        QTableWidgetItem* cantidadItem = new QTableWidgetItem(QString::number(fechasCantidad[i].second));
        ui->fechas_con_mas_publis_tabla->setItem(i, 1, cantidadItem);
    }

    // Limpiar la tabla antes de agregar nuevos datos
    ui->publis_con_mas_comentarios_tabla->clearContents();
    ui->publis_con_mas_comentarios_tabla->setRowCount(0);

    // Crear una lista para almacenar los detalles de cada publicación (Fecha, Correo, Número de comentarios)
    std::vector<std::tuple<std::string, std::string, int>> publisConMasComentarios;  // (Fecha, Correo, Numero de comentarios)

    // Llenar el vector con las publicaciones y la cantidad de comentarios
    obtenerDetallesComentariosDePublicaciones(arbolABB.getRaiz(), publisConMasComentarios);

    // Ordenar las publicaciones por la cantidad de comentarios en orden descendente
    std::sort(publisConMasComentarios.begin(), publisConMasComentarios.end(), [](const auto& a, const auto& b) {
        return std::get<2>(a) > std::get<2>(b);  // Ordenar por el número de comentarios
    });

    // Llenar la tabla con los detalles (Top 3)
    for (size_t i = 0; i < std::min(publisConMasComentarios.size(), size_t(3)); ++i) {
        ui->publis_con_mas_comentarios_tabla->insertRow(i);

        // Insertar la fecha en la primera columna
        QTableWidgetItem* fechaItem = new QTableWidgetItem(QString::fromStdString(std::get<0>(publisConMasComentarios[i])));
        ui->publis_con_mas_comentarios_tabla->setItem(i, 0, fechaItem);

        // Insertar el correo en la segunda columna
        QTableWidgetItem* correoItem = new QTableWidgetItem(QString::fromStdString(std::get<1>(publisConMasComentarios[i])));
        ui->publis_con_mas_comentarios_tabla->setItem(i, 1, correoItem);

        // Insertar la cantidad de comentarios en la tercera columna
        QTableWidgetItem* cantidadComentariosItem = new QTableWidgetItem(QString::number(std::get<2>(publisConMasComentarios[i])));
        ui->publis_con_mas_comentarios_tabla->setItem(i, 2, cantidadComentariosItem);
    }
}

// **Función para obtener los detalles de las publicaciones con más comentarios**
void Usuarios::obtenerDetallesComentariosDePublicaciones(NodoABB* nodo, std::vector<std::tuple<std::string, std::string, int>>& publisConMasComentarios)
{
    if (nodo) {
        // Recorrer el subárbol izquierdo
        obtenerDetallesComentariosDePublicaciones(nodo->izquierda, publisConMasComentarios);

        // Obtener los detalles para cada publicación en el nodo actual
        for (const auto& publicacion : nodo->publicaciones) {
            int idPublicacion = publicacion.getId();
            std::string fecha = publicacion.getFecha();
            std::string correo = publicacion.getCorreo();
            int cantidadComentarios = arbolComentarios_.getComentariosDePublicacion(idPublicacion).size();

            // Añadir los detalles (Fecha, Correo, Número de comentarios) al vector
            publisConMasComentarios.push_back(std::make_tuple(fecha, correo, cantidadComentarios));
        }

        // Recorrer el subárbol derecho
        obtenerDetallesComentariosDePublicaciones(nodo->derecha, publisConMasComentarios);
    }
}

// **Función para llenar las fechas y cantidad de publicaciones**
void Usuarios::llenarFechasCantidad(NodoABB* nodo, std::vector<std::pair<std::string, int>>& fechasCantidad)
{
    if (nodo) {
        // Recorrer el subárbol izquierdo
        llenarFechasCantidad(nodo->izquierda, fechasCantidad);

        // Añadir la fecha actual y la cantidad de publicaciones
        fechasCantidad.push_back({nodo->fecha, static_cast<int>(nodo->publicaciones.size())});

        // Recorrer el subárbol derecho
        llenarFechasCantidad(nodo->derecha, fechasCantidad);
    }
}

// **Función para obtener la cantidad de comentarios por publicación**
void Usuarios::obtenerCantidadComentariosDePublicaciones(NodoABB* nodo, std::vector<std::pair<int, int>>& publisConMasComentarios)
{
    if (nodo) {
        // Recorrer el subárbol izquierdo
        obtenerCantidadComentariosDePublicaciones(nodo->izquierda, publisConMasComentarios);

        // Obtener la cantidad de comentarios para cada publicación en el nodo actual
        for (const auto& publicacion : nodo->publicaciones) {
            int idPublicacion = publicacion.getId();
            int cantidadComentarios = arbolComentarios_.getComentariosDePublicacion(idPublicacion).size();

            // Añadir el ID de la publicación y la cantidad de comentarios al vector
            publisConMasComentarios.push_back({idPublicacion, cantidadComentarios});
        }

        // Recorrer el subárbol derecho
        obtenerCantidadComentariosDePublicaciones(nodo->derecha, publisConMasComentarios);
    }
}

void Usuarios::on_actualizarTablaRecomendados_boton_clicked()
{
    try {
        // Limpiar la tabla antes de agregar los datos nuevos
        ui->tablaRecomendados->setRowCount(0);

        // Suponiendo que ya tienes un método para obtener el correo o nombre de usuario actual
        std::string correoActual = correoActualUsuario_; // Asegúrate de que este valor sea correcto

        if (correoActual.empty()) {
            std::cout << "Correo actual está vacío." << std::endl;
            return;
        }

        std::cout << "Correo actual: " << correoActual << std::endl;

        // Variable para almacenar la cantidad de recomendaciones
        int cantidadRecomendaciones = 0;

        // Llamar al método recomendarAmigos del grafo
        std::string* recomendaciones = grafoNoDirigido.recomendarAmigos(correoActual, cantidadRecomendaciones);

        if (recomendaciones == nullptr || cantidadRecomendaciones == 0) {
            // No hay recomendaciones, tal vez mostrar un mensaje
            std::cout << "No se encontraron recomendaciones." << std::endl;
            return;
        }

        std::cout << "Cantidad de recomendaciones: " << cantidadRecomendaciones << std::endl;

        // Asegurarse de que la tabla tiene 4 columnas para nombre, apellido, correo, amigos en común
        if (ui->tablaRecomendados->columnCount() != 4) {
            ui->tablaRecomendados->setColumnCount(4);  // Asegúrate de que la tabla tenga 4 columnas
        }

        // Iterar sobre las recomendaciones y agregarlas a la tabla
        for (int i = 0; i < cantidadRecomendaciones; ++i) {
            std::cout << "Procesando recomendación: " << recomendaciones[i] << std::endl;

            // Buscar el usuario recomendado en el árbol AVL
            Usuario* usuarioRecomendado = listaUsuarios->buscarUsuarioPorCorreo(recomendaciones[i]);

            if (usuarioRecomendado == nullptr) {
                std::cout << "Usuario recomendado no encontrado: " << recomendaciones[i] << std::endl;
                continue;
            }

            std::cout << "Usuario encontrado: " << usuarioRecomendado->getNombre() << std::endl;

            // Obtener el número de amigos en común
            int amigosEnComun = grafoNoDirigido.obtenerAmigosEnComun(correoActual, recomendaciones[i]);
            std::cout << "Amigos en común: " << amigosEnComun << std::endl;

            // Insertar una nueva fila en la tabla
            int filaActual = ui->tablaRecomendados->rowCount();
            ui->tablaRecomendados->insertRow(filaActual);

            // Rellenar las columnas de la tabla
            ui->tablaRecomendados->setItem(filaActual, 0, new QTableWidgetItem(QString::fromStdString(usuarioRecomendado->getNombre())));
            ui->tablaRecomendados->setItem(filaActual, 1, new QTableWidgetItem(QString::fromStdString(usuarioRecomendado->getApellido())));
            ui->tablaRecomendados->setItem(filaActual, 2, new QTableWidgetItem(QString::fromStdString(recomendaciones[i]))); // El correo o nombre de usuario
            ui->tablaRecomendados->setItem(filaActual, 3, new QTableWidgetItem(QString::number(amigosEnComun)));
        }

        // Generar archivo DOT y PNG
        std::string nombreArchivoDot = "grafo_Amistades_Sugerencias.dot";  // Nombre del archivo DOT
        grafoNoDirigido.generarArchivoDOTEstilos(nombreArchivoDot, correoActual);  // Generar archivo DOT

        // Generar PNG a partir del archivo DOT
        std::string comando = "dot -Tpng " + nombreArchivoDot + " -o " + nombreArchivoDot + ".png";
        if (system(comando.c_str()) != 0) {
            std::cerr << "Error al generar el archivo PNG" << std::endl;
        }

        // Liberar la memoria de las recomendaciones
        delete[] recomendaciones;
    }
    catch (const std::exception& e) {
        std::cerr << "Error al mostrar publicaciones: " << e.what() << std::endl;
        QMessageBox::critical(this, "Error", "Hubo un problema al mostrar las publicaciones.");
        return;
    }
}



void Usuarios::on_VerAmistadySugerncias_boton_clicked()
{
    // Generar el archivo PNG del grafo
    std::string nombreArchivoPNG = "grafo_Amistades_Sugerencias.dot";  // Agregar la extensión .dot
    grafoNoDirigido.generarArchivoDOTEstilos(nombreArchivoPNG, correoActualUsuario_);

    // Convertir el nombre del archivo a QString
    QString imagePath = QString::fromStdString(nombreArchivoPNG + ".png");

    // Verificar si existe un layout anterior en el label y eliminarlo
    QLayout* existingLayout = ui->amistadesYsugerencias_label->layout(); // Usa el layout correspondiente
    if (existingLayout) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0))) {
            delete item->widget(); // Eliminar el widget asociado
            delete item;
        }
        delete existingLayout;
    }

    // Crear un nuevo layout para la imagen
    QVBoxLayout* newLayout = new QVBoxLayout();
    ui->amistadesYsugerencias_label->setLayout(newLayout);

    // Crear un QLabel para mostrar la imagen
    QLabel* imageLabel = new QLabel();
    QPixmap pixmap(imagePath);

    if (pixmap.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen" << imagePath;
        return;
    }

    // Ajustar el QLabel con el pixmap y redimensionar al tamaño de la imagen
    imageLabel->setPixmap(pixmap);
    imageLabel->resize(pixmap.size());

    // Crear un QScrollArea para la imagen
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Hacer que el contenido se ajuste con desplazamiento si es necesario
    scrollArea->setWidget(imageLabel);     // Establecer QLabel como el contenido de QScrollArea

    // Añadir el QScrollArea al layout
    newLayout->addWidget(scrollArea);
}

