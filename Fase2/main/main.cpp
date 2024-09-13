#include <QApplication>
#include "login.h"
#include "ListaUsuarios.h"
#include "listaDoblePublicacion.h"
#include "lista_solicitudes.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Crear instancias globales
    ListaUsuarios listaUsuarios;
    ListaDoblePublicacion listadoblepublicacion;
    ListaSolicitudes lista_solicitudes;

    // Pasar las instancias a Login
    Login w(&listaUsuarios, &listadoblepublicacion, &lista_solicitudes);
    w.show();

    return a.exec();
}
