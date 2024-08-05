#include <iostream>
#include <limits>

int main() {
    int opcion;

    do {
        std::cout << "MENU" << std::endl;
        std::cout << "1. Iniciar sesion" << std::endl;
        std::cout << "2. Registrarse" << std::endl;
        std::cout << "3. Informacion" << std::endl;
        std::cout << "4. Salir" << std::endl;
        std::cout << "Ingrese su opción: ";
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
            opcion = 0; 
        } else {
            switch (opcion) {
                case 1:
                    std::cout << "Ha seleccionado la opción 1." << std::endl;

                    break;
                case 2:
                    std::cout << "Ha seleccionado la opción 2." << std::endl;

                    break;
                case 3:
                    std::cout <<"-----------------Informacion del estudiante-----------------" << std::endl;
                    std::cout << "Nombre: Kevin Andrés Alvarez Herrera" << std::endl;
                    std::cout << "Carnet: 202203038" << std::endl;
                    std::cout << "Ingenieria en ciencias y sistemas" << std::endl;
                    std::cout << "Curso: Estructura de datos" << std::endl;
                    std::cout << "Seccion: C" << std::endl;
                    std::cout << "----------------------------------" << std::endl;
                    break;
                case 4:
                    std::cout << "Saliendo del programa..." << std::endl;
                    break;
                default:
                    std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
                    break;
            }
        }

        std::cout << std::endl;

    } while (opcion != 4);

    return 0;
}
