#include "arbolabb.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>

ArbolABB arbolABB;
// Constructor
ArbolABB::ArbolABB() : raiz(nullptr) {}

// Destructor
ArbolABB::~ArbolABB() {
}

NodoPublicacion::NodoPublicacion(const Publicacion& pub)
    : publicacion(pub), siguiente(nullptr) {}

NodoABB::NodoABB(int id_, const std::string& fecha_)
    : id(id_), fecha(fecha_), publicaciones(nullptr), izquierda(nullptr), derecha(nullptr), altura(1) {}

NodoABB::~NodoABB() {
    // Liberar la memoria de la lista de publicaciones si es necesario
    NodoPublicacion* actual = publicaciones;
    while (actual) {
        NodoPublicacion* temp = actual;
        actual = actual->siguiente; // Mover al siguiente nodo
        delete temp; // Liberar la memoria del nodo actual
    }
    // No es necesario liberar 'izquierda' y 'derecha' aquí, ya que son gestionados por el árbol
}
// Insertar una publicación en el ABB
void ArbolABB::insertarPublicacion(const Publicacion& publicacion) {
    raiz = insertarNodo(raiz, publicacion);
}

NodoABB* ArbolABB::insertarNodo(NodoABB* nodo, const Publicacion& publicacion) {
    std::string fechaConvertida = convertirFecha(publicacion.getFecha());

    if (!nodo) {
        int nuevoId = publicacion.getId();
        NodoABB* nuevoNodo = new NodoABB(nuevoId, fechaConvertida);
        nuevoNodo->publicaciones = new NodoPublicacion(publicacion); // Crear un nuevo nodo de publicación
        return nuevoNodo;
    }

    if (fechaConvertida < nodo->fecha) {
        nodo->izquierda = insertarNodo(nodo->izquierda, publicacion);
    } else if (fechaConvertida > nodo->fecha) {
        nodo->derecha = insertarNodo(nodo->derecha, publicacion);
    } else {
        // Agregar la nueva publicación a la lista enlazada
        NodoPublicacion* nuevoNodo = new NodoPublicacion(publicacion);
        nuevoNodo->siguiente = nodo->publicaciones; // Insertar al inicio
        nodo->publicaciones = nuevoNodo;
    }

    // Actualizar la altura del nodo
    actualizarAltura(nodo);

    // Obtener el factor de equilibrio
    int balance = obtenerBalance(nodo);

    // Rotaciones para mantener el equilibrio
    if (balance > 1 && fechaConvertida < nodo->izquierda->fecha) {
        return rotarDerecha(nodo);
    }
    if (balance < -1 && fechaConvertida > nodo->derecha->fecha) {
        return rotarIzquierda(nodo);
    }
    if (balance > 1 && fechaConvertida > nodo->izquierda->fecha) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && fechaConvertida < nodo->derecha->fecha) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

int ArbolABB::obtenerAltura(NodoABB* nodo) {
    return nodo ? nodo->altura : 0;
}

int ArbolABB::obtenerBalance(NodoABB* nodo) {
    return nodo ? obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha) : 0;
}

void ArbolABB::actualizarAltura(NodoABB* nodo) {
    nodo->altura = 1 + std::max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha));
}

NodoABB* ArbolABB::rotarDerecha(NodoABB* y) {
    NodoABB* x = y->izquierda;
    NodoABB* T2 = x->derecha;

    // Realizar la rotación
    x->derecha = y;
    y->izquierda = T2;

    // Actualizar alturas
    actualizarAltura(y);
    actualizarAltura(x);

    return x; // Nueva raíz
}

bool NodoArbolBComentario::compararComentarios(const Comentario& c1, const Comentario& c2) {
    if (c1.getFecha() < c2.getFecha()) {
        return true;
    } else if (c1.getFecha() == c2.getFecha()) {
        return c1.getHora() < c2.getHora();
    }
    return false;
}

NodoABB* ArbolABB::rotarIzquierda(NodoABB* x) {
    NodoABB* y = x->derecha;
    NodoABB* T2 = y->izquierda;

    // Realizar la rotación
    y->izquierda = x;
    x->derecha = T2;

    // Actualizar alturas
    actualizarAltura(x);
    actualizarAltura(y);

    return y; // Nueva raíz
}

// Buscar un nodo por fecha
NodoABB* ArbolABB::buscarNodo(NodoABB* nodo, const std::string& fecha) const {
    std::string fechaConvertida = convertirFecha(fecha);

    if (!nodo || nodo->fecha == fechaConvertida) {
        return nodo;
    }

    if (fechaConvertida < nodo->fecha) {
        return buscarNodo(nodo->izquierda, fecha);
    } else {
        return buscarNodo(nodo->derecha, fecha);
    }
}

void ArbolABB::agregarPublicacion(Publicacion**& publicaciones, int& cantidad, int& capacidad, const Publicacion& publicacion) const {
    if (cantidad >= capacidad) {
        // Duplicar la capacidad
        capacidad *= 2;
        Publicacion** nuevoArreglo = new Publicacion*[capacidad];
        for (int i = 0; i < cantidad; ++i) {
            nuevoArreglo[i] = publicaciones[i];
        }
        delete[] publicaciones; // Liberar el arreglo anterior
        publicaciones = nuevoArreglo; // Asignar el nuevo arreglo
    }
    // Crear una copia de la publicación
    publicaciones[cantidad] = new Publicacion(publicacion); // Asumiendo que Publicacion tiene un constructor de copia
    cantidad++;
}

// Recorrido en preorden que agrega publicaciones a la lista
void ArbolABB::recorrerPreOrder(NodoABB* nodo, ListaPublicaciones& publicaciones) const {
    if (nodo) {
        // Agregar las publicaciones del nodo actual a la lista
        for (NodoPublicacion* actual = nodo->publicaciones; actual; actual = actual->siguiente) {
            publicaciones.agregarPublicacion(actual->publicacion);
        }
        // Recorrer el subárbol izquierdo
        recorrerPreOrder(nodo->izquierda, publicaciones);
        // Recorrer el subárbol derecho
        recorrerPreOrder(nodo->derecha, publicaciones);
    }
}

// Recorrido en orden que agrega publicaciones a la lista
void ArbolABB::recorrerInOrder(NodoABB* nodo, ListaPublicaciones& publicaciones) const {
    if (nodo) {
        // Recorrer el subárbol izquierdo
        recorrerInOrder(nodo->izquierda, publicaciones);
        // Agregar las publicaciones del nodo actual a la lista
        for (NodoPublicacion* actual = nodo->publicaciones; actual; actual = actual->siguiente) {
            publicaciones.agregarPublicacion(actual->publicacion);
        }
        // Recorrer el subárbol derecho
        recorrerInOrder(nodo->derecha, publicaciones);
    }
}

// Recorrido postorden que agrega publicaciones a la lista
void ArbolABB::recorrerPostOrder(NodoABB* nodo, ListaPublicaciones& publicaciones) const {
    if (nodo) {
        // Recorrer el subárbol izquierdo
        recorrerPostOrder(nodo->izquierda, publicaciones);
        // Recorrer el subárbol derecho
        recorrerPostOrder(nodo->derecha, publicaciones);
        // Agregar las publicaciones del nodo actual a la lista
        for (NodoPublicacion* actual = nodo->publicaciones; actual; actual = actual->siguiente) {
            publicaciones.agregarPublicacion(actual->publicacion);
        }
    }
}

// Métodos públicos que llaman a los recorridos con la lista de publicaciones
void ArbolABB::recorrerPreOrder(ListaPublicaciones& publicaciones) const {
    recorrerPreOrder(raiz, publicaciones);
}

void ArbolABB::recorrerInOrder(ListaPublicaciones& publicaciones) const {
    recorrerInOrder(raiz, publicaciones);
}

void ArbolABB::recorrerPostOrder(ListaPublicaciones& publicaciones) const {
    recorrerPostOrder(raiz, publicaciones);
}

void ArbolABB::recorrerInOrder(NodoABB* nodo, ListaFechas& fechas) const {
    if (nodo) {
        // Recorrer el subárbol izquierdo
        recorrerInOrder(nodo->izquierda, fechas);

        // Agregar la fecha del nodo actual a la lista de fechas
        fechas.agregarFecha(nodo->fecha); // Asumiendo que hay un método agregarFecha en ListaFechas

        // Recorrer el subárbol derecho
        recorrerInOrder(nodo->derecha, fechas);
    }
}


// Liberar la memoria utilizada
void ArbolABB::liberarPublicaciones(Publicacion** publicaciones, int cantidad) const {
    for (int i = 0; i < cantidad; ++i) {
        delete publicaciones[i]; // Liberar cada Publicacion
    }
    delete[] publicaciones; // Liberar el arreglo
}


void ArbolABB::generateDotFile(const std::string& filename, const std::string& fechaBuscada) const {
    std::ofstream file(filename + ".dot");
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo DOT para escribir." << std::endl;
        return;
    }

    file << "digraph BSTTree {\n";
    file << "node [shape=circle];\n";

    generateDot(raiz, file, fechaBuscada);

    file << "}\n";
    file.close();

    // Convertir el archivo DOT a PNG usando el comando dot
    std::string comando = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
    int resultado = std::system(comando.c_str());

    if (resultado != 0) {
        std::cerr << "Error al convertir el archivo DOT a PNG." << std::endl;
    }
}

void ArbolABB::generateDot(NodoABB* nodo, std::ofstream& file, const std::string& fechaBuscada) const {
    if (nodo) {
        // Conectar los nodos del árbol con sus hijos
        if (nodo->izquierda) {
            file << "\"" << nodo->fecha << "\" -> \"" << nodo->izquierda->fecha << "\";\n";
            generateDot(nodo->izquierda, file, fechaBuscada);
        }
        if (nodo->derecha) {
            file << "\"" << nodo->fecha << "\" -> \"" << nodo->derecha->fecha << "\";\n";
            generateDot(nodo->derecha, file, fechaBuscada);
        }

        // Verificar si la fecha del nodo coincide con la fecha buscada
        if (nodo->fecha == fechaBuscada) {
            // Iterar sobre todas las publicaciones del nodo
            NodoPublicacion* actual = nodo->publicaciones; // Empezar en el primer nodo de la lista
            while (actual) {
                std::string idNodo = "pub_" + std::to_string(actual->publicacion.getId());  // Generar un ID único para el nodo de la publicación

                // Crear un nodo para la publicación con su información
                file << "\"" << idNodo << "\" [label=\"ID: " << actual->publicacion.getId()
                     << "\\nFecha: " << actual->publicacion.getFecha()
                     << "\\nHora: " << actual->publicacion.getHora()
                     << "\\nCorreo: " << actual->publicacion.getCorreo()
                     << "\\nContenido: " << actual->publicacion.getContenido() << "\"];\n";

                // Conectar el nodo de la fecha con cada publicación
                file << "\"" << nodo->fecha << "\" -> \"" << idNodo << "\";\n";

                actual = actual->siguiente; // Mover al siguiente nodo en la lista de publicaciones
            }
        }
    }
}


void ArbolABB::eliminarPublicacion(int id) {
    raiz = eliminarNodo(raiz, id);
}

NodoABB* ArbolABB::eliminarNodo(NodoABB* nodo, int id) {
    if (!nodo) {
        return nodo; // El árbol está vacío o el nodo no se encontró
    }

    // Buscar el nodo que se quiere eliminar por ID
    if (id < nodo->id) {
        nodo->izquierda = eliminarNodo(nodo->izquierda, id);
    } else if (id > nodo->id) {
        nodo->derecha = eliminarNodo(nodo->derecha, id);
    } else {
        // Nodo encontrado

        // Caso 1: Nodo con un solo hijo o sin hijos
        if (!nodo->izquierda) {
            NodoABB* temp = nodo->derecha;
            delete nodo; // Liberar la memoria
            return temp; // Retornar el hijo
        } else if (!nodo->derecha) {
            NodoABB* temp = nodo->izquierda;
            delete nodo; // Liberar la memoria
            return temp; // Retornar el hijo
        }

        // Caso 2: Nodo con dos hijos
        NodoABB* temp = buscarMinimo(nodo->derecha); // Encontrar el sucesor
        nodo->id = temp->id; // Copiar el valor del sucesor
        nodo->fecha = temp->fecha; // Copiar la fecha del sucesor (opcional)
        nodo->publicaciones = temp->publicaciones; // Copiar las publicaciones del sucesor
        nodo->derecha = eliminarNodo(nodo->derecha, temp->id); // Eliminar el sucesor
    }

    // Actualizar la altura del nodo
    actualizarAltura(nodo);

    // Obtener el factor de equilibrio
    int balance = obtenerBalance(nodo);

    // Rotaciones para mantener el equilibrio
    if (balance > 1 && obtenerBalance(nodo->izquierda) >= 0) {
        return rotarDerecha(nodo);
    }
    if (balance > 1 && obtenerBalance(nodo->izquierda) < 0) {
        nodo->izquierda = rotarIzquierda(nodo->izquierda);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && obtenerBalance(nodo->derecha) <= 0) {
        return rotarIzquierda(nodo);
    }
    if (balance < -1 && obtenerBalance(nodo->derecha) > 0) {
        nodo->derecha = rotarDerecha(nodo->derecha);
        return rotarIzquierda(nodo);
    }

    return nodo; // Retornar el nodo (sin cambios)
}

NodoABB* ArbolABB::buscarMinimo(NodoABB* nodo) const {
    NodoABB* actual = nodo;
    while (actual->izquierda != nullptr) {
        actual = actual->izquierda;
    }
    return actual;
}


// Convertir la fecha a un formato ordenado (YYYY/MM/DD)
std::string ArbolABB::convertirFecha(const std::string& fechaStr) const {
    int dia, mes, ano;
    char separador;
    std::istringstream iss(fechaStr);
    iss >> dia >> separador >> mes >> separador >> ano;

    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << ano << "/"
        << std::setw(2) << std::setfill('0') << mes << "/"
        << std::setw(2) << std::setfill('0') << dia;

    return oss.str();
}

// Destruir el árbol
void ArbolABB::destruirArbol(NodoABB* nodo) {
    if (nodo) {
        destruirArbol(nodo->izquierda);
        destruirArbol(nodo->derecha);
        delete nodo;
    }
}
