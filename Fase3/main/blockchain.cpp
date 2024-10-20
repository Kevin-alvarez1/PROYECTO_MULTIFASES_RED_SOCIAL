#include "blockchain.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <QApplication>
#include <QString>
#include <QCryptographicHash>
#include <fstream>

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
