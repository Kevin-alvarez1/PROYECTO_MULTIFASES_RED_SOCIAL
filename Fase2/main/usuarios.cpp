#include "usuarios.h"
#include "ui_usuarios.h"
#include "login.h"
#include "QMessageBox"
#include "matrizdispersa.h"
#include "arbolabb.h"
#include <QDialog>
#include <QVBoxLayout>
#include <set>
extern MatrizDispersa matrizDispersa;
extern ArbolABB arbolABB;

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
            Receptor solicitud = copiaPilaReceptor.peek();

            // Verifica si la solicitud está en estado "PENDIENTE"
            if (lista_solicitudes->existeSolicitudEnEstado(solicitud.getEmisor(), correoActual.toStdString(), "PENDIENTE")) {

                // Obtener la lista de amigos del usuario actual desde la matrizDispersa
                std::vector<std::string> amigos = matrizDispersa.obtenerAmigos(correoActual.toStdString());

                // Verificar si el emisor ya es amigo del usuario actual
                if (std::find(amigos.begin(), amigos.end(), solicitud.getEmisor()) == amigos.end()) {
                    // Si no son amigos, agregar la solicitud a la lista para mostrarla
                    solicitudesRecibidas.push_back(solicitud);
                }
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
                    matrizDispersa.insertarRelacion(correoEmisor, correoReceptor);

                    // Mostrar un mensaje de confirmación
                    QMessageBox::information(this, "Solicitud Aceptada",
                                             "La solicitud de " + QString::fromStdString(correoEmisor) + " ha sido aceptada.");

                    matrizDispersa.mostrarMatriz();
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


void Usuarios::on_fecha_filtro_publis_boton_clicked()
{

}

void Usuarios::on_crear_nueva_publi_boton_clicked()
{
    if (!CrearPublicacion) {
        CrearPublicacion = new class CrearPublicacion(correoActualUsuario_, listadoblepublicacion, this);
    }

    CrearPublicacion->show();
    this->hide();
}


void Usuarios::on_aplicar_orden_publis_boton_clicked()
{
    try {
        QString criterioOrdenQt = ui->ordenPublicaciones_comboBox->currentText();
        std::string criterioOrden = criterioOrdenQt.toUtf8().constData();

        // Verificar que listadoblepublicacion esté inicializado
        if (!listadoblepublicacion) {
            QMessageBox::critical(this, "Error", "ListaDoblePublicacion no está inicializado.");
            return;
        }

        std::string correoUsuario = correoActualUsuario_;
        std::cout << "Publicaciones del usuario " << correoUsuario << ":" << std::endl;

        // Limpiar el layout de publicaciones anteriores
        QLayout* layout = ui->publicaciones_frame->layout();
        if (layout != nullptr) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
        } else {
            layout = new QVBoxLayout(ui->publicaciones_frame);
            ui->publicaciones_frame->setLayout(layout);
        }

        // Obtener publicaciones en orden
        std::vector<Publicacion> publicaciones_arbol = listadoblepublicacion->mostrarPublicacionesYAmigos(correoUsuario, matrizDispersa, arbolABB);

        // Depuración: verificar cuántas publicaciones se encontraron
        std::cout << "Total de publicaciones encontradas: " << publicaciones_arbol.size() << std::endl;

        std::set<int> idsMostradas; // Para llevar un registro de los IDs mostrados

        for (const auto& publicacion : publicaciones_arbol) {
            // Depuración: mostrar la información de cada publicación
            std::cout << "Publicación ID: " << publicacion.getId() << ", correo: " << publicacion.getCorreo() << std::endl;

            // Verificar si el ID ya ha sido mostrado
            if (idsMostradas.find(publicacion.getId()) == idsMostradas.end()) {
                // Agregar el ID a la lista de mostrados
                idsMostradas.insert(publicacion.getId());

                // Crear un nuevo panel para cada publicación
                QFrame* panel = new QFrame(ui->publicaciones_frame);
                panel->setFrameShape(QFrame::StyledPanel);
                panel->setFrameShadow(QFrame::Raised);

                // Layout para el panel
                QVBoxLayout* panelLayout = new QVBoxLayout(panel);

                // Crear y configurar el label para mostrar el ID de la publicación
                QLabel* label = new QLabel(QString("ID de la publicación: %1").arg(publicacion.getId()), panel);
                panelLayout->addWidget(label);

                // Añadir el panel al layout principal
                layout->addWidget(panel);
            }
        }

        ui->publicaciones_frame->update();
        ui->publicaciones_frame->repaint();

    } catch (const std::exception& e) {
        std::cerr << "Error al mostrar publicaciones: " << e.what() << std::endl;
        QMessageBox::critical(this, "Error", "Hubo un problema al mostrar las publicaciones.");
        return;
    }

    QMessageBox::information(this, "Publicaciones", "Se han mostrado todas las publicaciones del usuario.");
}




void Usuarios::on_generar_bst_reporte_boton_clicked() {
    // Generar el archivo DOT para el árbol
    arbolABB.generateDotFile("ArbolBB");

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
