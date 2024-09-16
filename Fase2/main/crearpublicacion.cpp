#include "crearpublicacion.h"
#include "ui_crearpublicacion.h"
#include <QMessageBox>
#include "usuarios.h"
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>

CrearPublicacion::CrearPublicacion(std::string correoUsuario, ListaDoblePublicacion *listaDoblePublicacion, QWidget *parent)
    : QDialog(parent),
    correoActualUsuario_(correoUsuario),
    usuarioWindow(nullptr),
    listaDoblePublicacion(listaDoblePublicacion)
{
    ui = new Ui::CrearPublicacion;
    ui->setupUi(this);
    ui->correo_publicacion_txt->setText(QString::fromStdString(correoActualUsuario_));

}


CrearPublicacion::~CrearPublicacion()
{
    delete ui;
}

void CrearPublicacion::on_crearPublicacion_boton_clicked()
{
    // Recuperar los datos del formulario
    QString contenido = ui->contenido_publicacion_txt->toPlainText();
    QString fecha = ui->fecha_publicacion_txt->text();
    QString hora = ui->hora_publicacion_txt->text();

    // Validar los datos
    if (contenido.isEmpty() || fecha.isEmpty() || hora.isEmpty()) {
        QMessageBox::warning(this, "Advertencia", "Por favor, complete todos los campos.");
        return;
    }

    // Crear una nueva publicación con un nuevo ID
    int nuevoId = listaDoblePublicacion->obtenerNuevoId();  // Obtener un nuevo ID único
    Publicacion nuevaPublicacion(nuevoId, correoActualUsuario_, contenido.toStdString(), fecha.toStdString(), hora.toStdString());

    // Agregar la publicación a la lista de publicaciones
    listaDoblePublicacion->agregarPublicacion(nuevaPublicacion);

    // Informar al usuario
    QMessageBox::information(this, "Éxito", "Publicación creada exitosamente.");

}

void CrearPublicacion::on_seleccionarImagen_boton_clicked()
{
    // Abre un diálogo para seleccionar un archivo de imagen
    QString fileName = QFileDialog::getOpenFileName(this, tr("Seleccionar Imagen"), "", tr("Archivos de Imagen (*.png *.jpg *.jpeg *.bmp)"));

    // Verifica si se seleccionó un archivo
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, tr("Advertencia"), tr("No se seleccionó ninguna imagen."));
        return; // Si no se seleccionó nada, salimos de la función
    }

    // Cargar la imagen en un QPixmap
    QPixmap imagen;
    if (!imagen.load(fileName)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo cargar la imagen seleccionada."));
        return;
    }

    // Obtener la ruta del directorio de la aplicación, específicamente la carpeta "Imagenes"
    QDir directorioBase(QDir::currentPath());
    QString rutaImagenes = directorioBase.filePath("Imagenes");

    // Depuración: Mostrar la ruta base y la ruta de destino
    qDebug() << "Ruta base:" << directorioBase.absolutePath();
    qDebug() << "Ruta de imágenes:" << rutaImagenes;

    // Crear el directorio si no existe
    if (!QDir(rutaImagenes).exists()) {
        if (!QDir().mkpath(rutaImagenes)) {
            QMessageBox::warning(this, tr("Error"), tr("No se pudo crear el directorio de destino."));
            return;
        }
    }

    // Obtener el nombre del archivo y la ruta de destino
    QString nombreImagen = QFileInfo(fileName).fileName();  // Obtener solo el nombre del archivo
    QString archivoDestino = QDir(rutaImagenes).filePath(nombreImagen);  // Crear la ruta destino

    // Asegurarse de que el nombre del archivo sea único
    int contador = 1;
    while (QFile::exists(archivoDestino)) {
        QString nombreBase = QFileInfo(nombreImagen).completeBaseName();
        QString extension = QFileInfo(nombreImagen).suffix();
        QString nuevoNombre = QString("%1_%2.%3").arg(nombreBase).arg(contador).arg(extension);
        archivoDestino = QDir(rutaImagenes).filePath(nuevoNombre);
        contador++;
    }

    // Copiar el archivo a la carpeta destino
    if (!QFile::copy(fileName, archivoDestino)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo guardar la imagen en la carpeta destino."));
        return;
    }

    // Redimensionar la imagen para ajustarse al tamaño del visualizador
    int width = ui->visualizador_imagenes->width();  // Usa el puntero ui para acceder al QLabel
    int height = ui->visualizador_imagenes->height();
    QPixmap imagenRedimensionada = imagen.scaled(width, height, Qt::KeepAspectRatio);

    // Mostrar la imagen en el QLabel
    ui->visualizador_imagenes->setPixmap(imagenRedimensionada);

    // Confirmación de guardado
    QMessageBox::information(this, tr("Éxito"), tr("La imagen se ha guardado correctamente."));
}

void CrearPublicacion::on_cancelar_boton_clicked()
{
    if (parentWidget()) {
        parentWidget()->show();
    }
    this->close();
}


