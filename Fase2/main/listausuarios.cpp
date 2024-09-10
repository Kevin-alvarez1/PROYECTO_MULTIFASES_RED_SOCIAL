// ListaUsuarios.cpp
#include "ListaUsuarios.h"
#include <iostream>
#include <fstream>

ListaUsuarios::ListaUsuarios() : raiz(nullptr) {}

ListaUsuarios::~ListaUsuarios() {
    eliminarArbol(raiz);
}

void ListaUsuarios::eliminarArbol(NodoAVL* nodo) {
    if (nodo != nullptr) {
        eliminarArbol(nodo->izquierdo);
        eliminarArbol(nodo->derecho);
        delete nodo;
    }
}

int ListaUsuarios::altura(NodoAVL* nodo) const {
    return (nodo ? nodo->altura : 0);
}

int ListaUsuarios::balance(NodoAVL* nodo) const {
    return (nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0);
}

NodoAVL* ListaUsuarios::rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierdo;
    NodoAVL* T2 = x->derecho;

    x->derecho = y;
    y->izquierdo = T2;

    y->altura = std::max(altura(y->izquierdo), altura(y->derecho)) + 1;
    x->altura = std::max(altura(x->izquierdo), altura(x->derecho)) + 1;

    return x;
}

NodoAVL* ListaUsuarios::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecho;
    NodoAVL* T2 = y->izquierdo;

    y->izquierdo = x;
    x->derecho = T2;

    x->altura = std::max(altura(x->izquierdo), altura(x->derecho)) + 1;
    y->altura = std::max(altura(y->izquierdo), altura(y->derecho)) + 1;

    return y;
}

NodoAVL* ListaUsuarios::insertar(NodoAVL* nodo, const Usuario& usuario) {
    if (nodo == nullptr) {
        return new NodoAVL(usuario);
    }

    if (usuario.getCorreo() < nodo->usuario.getCorreo()) {
        nodo->izquierdo = insertar(nodo->izquierdo, usuario);
    } else if (usuario.getCorreo() > nodo->usuario.getCorreo()) {
        nodo->derecho = insertar(nodo->derecho, usuario);
    } else {
        return nodo; // El correo ya existe
    }

    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;

    int bal = balance(nodo);

    if (bal > 1 && usuario.getCorreo() < nodo->izquierdo->usuario.getCorreo()) {
        return rotarDerecha(nodo);
    }
    if (bal < -1 && usuario.getCorreo() > nodo->derecho->usuario.getCorreo()) {
        return rotarIzquierda(nodo);
    }
    if (bal > 1 && usuario.getCorreo() > nodo->izquierdo->usuario.getCorreo()) {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }
    if (bal < -1 && usuario.getCorreo() < nodo->derecho->usuario.getCorreo()) {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

void ListaUsuarios::agregarUsuario(const Usuario& usuario) {
    if (buscarUsuarioPorCorreo(usuario.getCorreo()) != nullptr) {
        std::cerr << "Usuario con correo " << usuario.getCorreo() << " ya existe." << std::endl;
        return;
    }

    raiz = insertar(raiz, usuario);
}

NodoAVL* ListaUsuarios::buscar(NodoAVL* nodo, const std::string& correo) const {
    if (nodo == nullptr || nodo->usuario.getCorreo() == correo) {
        return nodo;
    }

    if (correo < nodo->usuario.getCorreo()) {
        return buscar(nodo->izquierdo, correo);
    }

    return buscar(nodo->derecho, correo);
}

Usuario* ListaUsuarios::buscarUsuarioPorCorreo(const std::string& correo) const {
    NodoAVL* nodo = buscar(raiz, correo);
    return (nodo ? &nodo->usuario : nullptr);
}

NodoAVL* ListaUsuarios::minimoNodo(NodoAVL* nodo) {
    NodoAVL* actual = nodo;
    while (actual->izquierdo != nullptr) {
        actual = actual->izquierdo;
    }
    return actual;
}

NodoAVL* ListaUsuarios::eliminar(NodoAVL* nodo, const std::string& correo) {
    if (nodo == nullptr) {
        return nodo;
    }

    if (correo < nodo->usuario.getCorreo()) {
        nodo->izquierdo = eliminar(nodo->izquierdo, correo);
    } else if (correo > nodo->usuario.getCorreo()) {
        nodo->derecho = eliminar(nodo->derecho, correo);
    } else {
        if (nodo->izquierdo == nullptr) {
            NodoAVL* temp = nodo->derecho;
            delete nodo;
            return temp;
        } else if (nodo->derecho == nullptr) {
            NodoAVL* temp = nodo->izquierdo;
            delete nodo;
            return temp;
        }

        NodoAVL* temp = minimoNodo(nodo->derecho);
        nodo->usuario = temp->usuario;
        nodo->derecho = eliminar(nodo->derecho, temp->usuario.getCorreo());
    }

    if (nodo == nullptr) {
        return nodo;
    }

    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;

    int bal = balance(nodo);

    if (bal > 1 && balance(nodo->izquierdo) >= 0) {
        return rotarDerecha(nodo);
    }
    if (bal > 1 && balance(nodo->izquierdo) < 0) {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }
    if (bal < -1 && balance(nodo->derecho) <= 0) {
        return rotarIzquierda(nodo);
    }
    if (bal < -1 && balance(nodo->derecho) > 0) {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

void ListaUsuarios::borrarUsuarioPorCorreo(const std::string& correo) {
    raiz = eliminar(raiz, correo);
}

void ListaUsuarios::cargarUsuariosDesdeJson(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo JSON." << std::endl;
        return;
    }

    try {
        nlohmann::json jsonData;
        archivo >> jsonData;

        for (const auto& item : jsonData) {
            // Asegúrate de que las claves coincidan con las del JSON
            std::string nombre = item.at("nombres").get<std::string>();
            std::string apellido = item.at("apellidos").get<std::string>();
            std::string fecha_de_nacimiento = item.at("fecha_de_nacimiento").get<std::string>();
            std::string correo = item.at("correo").get<std::string>();
            std::string contrasena = item.at("contraseña").get<std::string>();

            Usuario usuario(nombre, apellido, fecha_de_nacimiento, correo, contrasena);
            agregarUsuario(usuario);
        }
    } catch (const nlohmann::json::exception &e) {
        std::cerr << "Error al procesar el archivo JSON: " << e.what() << std::endl;
    }
}


void ListaUsuarios::generateDot(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "digraph G {" << std::endl;
        file << "node [shape=record];" << std::endl;
        file << "rankdir=TB;" << std::endl;  // Cambio de dirección para árbol

        std::function<void(NodoAVL*, int)> printNode = [&](NodoAVL* nodo, int id) {
            if (nodo != nullptr) {
                int leftId = id * 2 + 1;
                int rightId = id * 2 + 2;
                file << "node" << id << " [label=\"{Nombre: " << nodo->usuario.getNombre() << "\\n"
                     << "Correo: " << nodo->usuario.getCorreo() << "}\"];" << std::endl;
                if (nodo->izquierdo != nullptr) {
                    file << "node" << id << " -> node" << leftId << ";" << std::endl;
                    printNode(nodo->izquierdo, leftId);
                }
                if (nodo->derecho != nullptr) {
                    file << "node" << id << " -> node" << rightId << ";" << std::endl;
                    printNode(nodo->derecho, rightId);
                }
            }
        };

        printNode(raiz, 0);

        file << "}" << std::endl;
        file.close();
    } else {
        std::cerr << "No se pudo abrir el archivo" << std::endl;
    }
}

bool ListaUsuarios::buscarUsuarioPorCorreoyContrasena(const std::string& correo, const std::string& contrasena) const {
    Usuario* usuario = buscarUsuarioPorCorreo(correo);
    if (usuario && usuario->getContrasena() == contrasena) {
        std::cout << "Usuario encontrado: " << usuario->getNombre() << std::endl;
        return true;
    }
    std::cout << "Usuario no encontrado." << std::endl;
    return false;
}

void ListaUsuarios::renderGraphviz(const std::string& dotFilename, const std::string& imageFilename) const {
    generateDot(dotFilename);
    std::string command = "dot -Tpng " + dotFilename + " -o " + imageFilename;
    system(command.c_str());
}
bool ListaUsuarios::usuarioDuplicado(const std::string &correo) const
{
    NodoAVL* actual = raiz;
    while (actual != nullptr)
    {
        if (correo == actual->usuario.getCorreo())
        {
            return true; // El usuario con el correo especificado ya existe
        }
        else if (correo < actual->usuario.getCorreo())
        {
            actual = actual->izquierdo; // Buscar en el subárbol izquierdo
        }
        else
        {
            actual = actual->derecho; // Buscar en el subárbol derecho
        }
    }
    return false; // No se encontró el usuario
}
Usuario ListaUsuarios::mostrarDatosPorCorreo(const std::string &correo) const
{
    NodoAVL* actual = raiz;
    while (actual != nullptr)
    {
        if (correo == actual->usuario.getCorreo())
        {
            return actual->usuario; // Usuario encontrado
        }
        else if (correo < actual->usuario.getCorreo())
        {
            actual = actual->izquierdo; // Buscar en el subárbol izquierdo
        }
        else
        {
            actual = actual->derecho; // Buscar en el subárbol derecho
        }
    }

    // Usuario no encontrado; devolver un objeto Usuario con valores predeterminados
    return Usuario("", "", "", "", ""); // Puedes ajustar esto según tu diseño
}
