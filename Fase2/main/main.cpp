#include <QApplication>
#include "login.h"
#include "ListaUsuarios.h"

// Crear una instancia global de ListaUsuarios
ListaUsuarios listaUsuarios;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login w(&listaUsuarios);
    w.show();

    return a.exec();
}
