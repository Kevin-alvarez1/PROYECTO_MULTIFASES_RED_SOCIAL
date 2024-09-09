#ifndef REGISTRARSE_H
#define REGISTRARSE_H

#include <QDialog>
#include "ListaUsuarios.h" // Incluir el encabezado de ListaUsuarios

class Login;
namespace Ui {
class Registrarse;
}


class Registrarse : public QDialog
{
    Q_OBJECT

public:
    explicit Registrarse(ListaUsuarios *listaUsuarios, QWidget *parent = nullptr);
    ~Registrarse();

private slots:
    void on_Registrar_boton_clicked();

private:
    Ui::Registrarse *ui;
    ListaUsuarios *listaUsuarios; // Cambiar a puntero
    Login *login;

};

#endif // REGISTRARSE_H
