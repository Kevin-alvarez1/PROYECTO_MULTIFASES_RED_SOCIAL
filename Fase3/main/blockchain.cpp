#include "blockchain.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <QApplication>
#include <QString>
#include <QCryptographicHash>
#include <fstream>
#include <filesystem>
#include "json.hpp"

Blockchain blockchain;

Block::Block(int idx, const std::string& data, const std::string& previousHash)
    : index(idx), data(data), previousHash(previousHash), nonce(0) {
    timestamp = getCurrentTime();
    hash = calculateHash();
}

std::string Block::calculateHash() const {
    std::ostringstream oss;
    oss << index << timestamp << data << nonce << previousHash;

    // Convertir el contenido en una cadena de tipo QString para usarlo en QCryptographicHash
    QString input = QString::fromStdString(oss.str());

    // Crear un hash SHA-256 usando QCryptographicHash
    QByteArray hash = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha256);

    // Convertir el hash en una cadena hexadecimal
    QString hexString = hash.toHex();

    return hexString.toStdString();  // Convertir de QString a std::string
}

void Blockchain::reconstruirDesdeJSON(const std::string& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                nlohmann::json blockJson;
                file >> blockJson;

                // Leer los campos del JSON
                int index = blockJson["index"];
                std::string timestamp = blockJson["timestamp"]; // Si necesitas el timestamp
                int nonce = blockJson["nonce"];

                // Acceder a los datos de la publicación
                std::string contenido = blockJson["data"]["Publicacion:"]["contenido"];
                std::string correo = blockJson["data"]["Publicacion:"]["correo"];
                std::string fecha = blockJson["data"]["Publicacion:"]["fecha"];
                std::string hora = blockJson["data"]["Publicacion:"]["hora"];
                int id = blockJson["data"]["Publicacion:"]["id"];

                // Acceder a los comentarios
                std::vector<std::string> comentarios;
                for (const auto& comentario : blockJson["data"]["Comentarios"]) {
                    comentarios.push_back(comentario["contenido"]); // Guarda solo el contenido del comentario
                }

                // Construir el nuevo bloque
                std::string previousHash = blockJson["previousHash"];
                std::string hash = blockJson["hash"];

                // Aquí puedes crear una estructura de datos adecuada para la publicación
                // o un bloque que contenga todos los datos que necesitas

                Block newBlock(index, contenido, previousHash);
                newBlock.nonce = nonce;
                newBlock.hash = hash;

                // Aquí puedes decidir cómo manejar los comentarios si son parte de tu lógica
                // Por ejemplo, si deseas guardarlos en el bloque, puedes hacerlo aquí

                addBlock(newBlock); // Agrega el bloque a la blockchain
                file.close();
            } else {
                std::cerr << "Error: No se pudo abrir el archivo " << entry.path() << " para leer." << std::endl;
            }
        }
    }
}


void Blockchain::addBlock(const Block& block) {
    // Crear un nuevo nodo y enlazarlo al final de la lista
    BlockNode* newNode = new BlockNode{block, nullptr};

    // Si la cadena está vacía, el nuevo nodo será el cabeza
    if (head == nullptr) {
        head = newNode;
    } else {
        // Si ya hay bloques, recorrer hasta el final
        BlockNode* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;  // Añadir el nuevo nodo al final de la lista
    }

    chainSize++;  // Incrementar el tamaño de la cadena
}


void Blockchain::addBlockFromJSON(const Block& block) {
    // Añadir el bloque reconstruido a la cadena de bloques
    BlockNode* nuevoNodo = new BlockNode{block, nullptr};
    if (!head) {
        head = nuevoNodo;
    } else {
        BlockNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = nuevoNodo;
    }
    chainSize++;
}


void Blockchain::generarGraficoBlockchain(const std::string& directory, const std::string& filename) {
    // Cargar bloques desde archivos JSON
    reconstruirDesdeJSON(directory);

    // Nombre del archivo DOT
    std::string dotFilename = filename + ".dot";
    // Nombre del archivo PNG
    std::string pngFilename = filename + ".png";

    std::ofstream dotFile(dotFilename);
    if (!dotFile.is_open()) {
        std::cerr << "No se pudo abrir el archivo DOT para escribir." << std::endl;
        return;
    }

    // Escribir la cabecera del archivo DOT
    dotFile << "digraph Blockchain {\n";
    dotFile << "    node [shape=record, style=filled, fillcolor=lightgrey];\n";

    // Recorrer la blockchain
    BlockNode* actual = head;
    while (actual != nullptr) {
        const Block& block = actual->block;

        // Crear una etiqueta para cada bloque con su índice, hash y datos
        dotFile << "    block" << block.index << " [label=\"{Index: " << block.index
                << "|Hash: " << block.hash.substr(0, 8) << "...|PrevHash: " << block.previousHash.substr(0, 8)
                << "...|Nonce: " << block.nonce << "|Data: " << block.data << "}\"];\n";

        // Conectar el bloque actual con el bloque anterior
        if (actual->block.index > 0) {
            dotFile << "    block" << block.index << " -> block" << block.index - 1 << ";\n";
        }

        // Avanzar al siguiente bloque
        actual = actual->next;
    }

    // Cerrar la estructura del archivo DOT
    dotFile << "}\n";
    dotFile.close();

    // Ejecutar el comando de Graphviz para generar el archivo PNG
    std::string command = "dot -Tpng " + dotFilename + " -o " + pngFilename;
    system(command.c_str());

    std::cout << "Archivo DOT generado: " << dotFilename << std::endl;
    std::cout << "Imagen PNG generada: " << pngFilename << std::endl;
}

// Minar el bloque buscando un hash que comience con "difficulty" ceros
void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0');  // Cadena que representa el objetivo, ej: "0000"

    do {
        nonce++;
        hash = calculateHash();
    } while (hash.substr(0, difficulty) != target);  // Seguir intentando hasta que el hash tenga el prefijo requerido

    std::cout << "Bloque minado: " << hash << std::endl;
}

std::string Block::toJSON() const {
    std::ostringstream oss;
    oss << "{\n";
    oss << "  \"index\": " << index << ",\n";
    oss << "  \"timestamp\": \"" << timestamp << "\",\n";
    oss << "  \"nonce\": " << nonce << ",\n";

    oss << "  \"data\": " << data << ",\n";

    oss << "  \"previousHash\": \"" << previousHash << "\",\n";
    oss << "  \"hash\": \"" << hash << "\"\n";
    oss << "}";
    return oss.str();
}

// Guardar el bloque en un archivo
void Block::saveToFile(const std::string& directory) const {
    std::string fileName = directory + "/block_" + std::to_string(index) + ".json";
    std::ofstream file(fileName);
    if (file.is_open()) {
        file << toJSON();
        file.close();
    } else {
        std::cerr << "Error al abrir el archivo para escribir el bloque." << std::endl;
    }
}

Blockchain::Blockchain() : head(nullptr), difficulty(4), chainSize(0) {
    // Crear el bloque génesis
    head = new BlockNode{Block(0, "Bloque génesis", "0000"), nullptr};
    chainSize = 1;
}

// Destructor: liberar todos los nodos de la lista
Blockchain::~Blockchain() {
    clear();
}

// Obtener el timestamp en formato DD-MM-YY::HH:MM:SS
std::string Block::getCurrentTime() const {
    std::time_t now = std::time(nullptr);
    std::tm* tmPtr = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(tmPtr, "%d-%m-%Y::%H:%M:%S");
    return oss.str();
}

void Blockchain::clear() {
    BlockNode* current = head;
    while (current != nullptr) {
        BlockNode* temp = current;
        current = current->next;
        delete temp;  // Liberar el nodo actual
    }
    head = nullptr;
    chainSize = 0;
}

// Obtener el último bloque de la cadena
Block Blockchain::getLastBlock() const {
    BlockNode* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    return current->block;
}

// Agregar un nuevo bloque (publicación/comentario)
void Blockchain::addBlock(const std::string& data) {
    Block lastBlock = getLastBlock();
    Block newBlock(chainSize, data, lastBlock.hash);  // Crear un nuevo bloque
    newBlock.mineBlock(difficulty);                  // Minar el nuevo bloque

    // Crear un nuevo nodo y enlazarlo al final de la lista
    BlockNode* newNode = new BlockNode{newBlock, nullptr};

    BlockNode* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = newNode;  // Añadir el nuevo nodo al final de la lista

    chainSize++;              // Incrementar el tamaño de la cadena
}

// Mostrar la cadena completa
void Blockchain::printBlockchain() const {
    BlockNode* current = head;
    while (current != nullptr) {
        const Block& block = current->block;
        std::cout << "Bloque #" << block.index << std::endl;
        std::cout << "Timestamp: " << block.timestamp << std::endl;
        std::cout << "Datos: " << block.data << std::endl;
        std::cout << "Nonce: " << block.nonce << std::endl;
        std::cout << "Hash: " << block.hash << std::endl;
        std::cout << "Hash anterior: " << block.previousHash << std::endl;
        std::cout << "--------------------------" << std::endl;
        current = current->next;
    }
}
