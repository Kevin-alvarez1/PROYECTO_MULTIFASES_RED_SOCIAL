#include <iostream>

int main() {
    int opcion;

    do {
        // Mostrar el menú
        std::cout << "MENU" << std::endl;
        std::cout << "1. Iniciar sesion" << std::endl;
        std::cout << "2. Registrarse" << std::endl;
        std::cout << "3. Informacion" << std::endl;
        std::cout << "4. Salir" << std::endl;
        std::cout << "Ingrese su opción: ";
        std::cin >> opcion;

        // Realizar acciones según la opción seleccionada
        switch (opcion) {
            case 1:
                std::cout << "Ha seleccionado la opción 1." << std::endl;
                // Agrega aquí el código para la opción 1
                
                break;
            case 2:
                std::cout << "Ha seleccionado la opción 2." << std::endl;
                // Agrega aquí el código para la opción 2
                break;
            case 3:
                std::cout <<"-----------------Informacion del estudiante-----------------" << std::endl;
                std::cout << "Nombre: Kevin Andrés Alvarez Herrera" << std::endl;
                std::cout << "Carnet: 202203038" << std::endl;
                std::cout << "Ingenieria en ciencias y sistemas" << std::endl;
                std::cout << "Curso: Estructura de datos" << std::endl;
                std::cout << "Seccion: C" << std::endl;
                std::cout << "----------------------------------" << std::endl;
                // Agrega aquí el código para la opción 3
                break;
            case 4:
                std::cout << "Saliendo del programa..." << std::endl;
                break;
            default:
                std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
                break;
        }

        std::cout << std::endl;

    } while (opcion != 4);

    return false;  

}