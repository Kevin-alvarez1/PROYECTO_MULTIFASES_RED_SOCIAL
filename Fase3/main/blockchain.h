#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>

class Block {
public:
    int index;
    std::string data;
    std::string timestamp;
    std::string previousHash;
    std::string hash;
    int nonce;

    Block(int idx, const std::string& data, const std::string& previousHash);

    // Función para calcular el hash del bloque
    std::string calculateHash() const;

    // Función para minar el bloque
    void mineBlock(int difficulty);
    void saveToFile(const std::string& directory) const;
    std::string toJSON() const;
private:
    // Obtener la fecha y hora actual como string
    std::string getCurrentTime() const;
};

class Blockchain {
private:
    struct BlockNode {
        Block block;         // Contendrá el bloque
        BlockNode* next;     // Apunta al siguiente bloque
    };

    BlockNode* head;         // Apuntador al primer bloque
    int chainSize;           // Tamaño de la cadena de bloques (cantidad de bloques)

public:
    Blockchain();            // Constructor
    ~Blockchain();           // Destructor
    int difficulty;          // Dificultad de la prueba de trabajo (cantidad de ceros requeridos en el hash)

    // Obtener el último bloque de la cadena
    Block getLastBlock() const;

    // Agregar un nuevo bloque (publicación/comentario)
    void addBlock(const std::string& data);

    // Mostrar la cadena completa
    void printBlockchain() const;

private:
    // Función auxiliar para liberar la memoria de los nodos
    void clear();
};

#endif // BLOCKCHAIN_H
