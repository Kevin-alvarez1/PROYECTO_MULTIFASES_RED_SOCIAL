#ifndef USUARIOS_H
#define USUARIOS_H

#include <QDialog>

namespace Ui {
class Usuarios;
}

class Usuarios : public QDialog
{
    Q_OBJECT

public:
    explicit Usuarios(QWidget *parent = nullptr);
    ~Usuarios();

private:
    Ui::Usuarios *ui;
};

#endif // USUARIOS_H
