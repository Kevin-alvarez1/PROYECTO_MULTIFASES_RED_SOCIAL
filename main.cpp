#include <iostream>
#include <limits>
#include "usuarios.cpp" // Asegúrate de que usuarios.cpp no tenga una definición de main()

std::string admin_correo = "admin@gmail.com";
std::string admin_contrasena = "EDD2S2024";

int main() {
    int opcion;

    do {
        std::cout << "MENU" << std::endl;
        std::cout << "1. Iniciar sesión" << std::endl;
        std::cout << "2. Registrarse" << std::endl;
        std::cout << "3. Información" << std::endl;
        std::cout << "4. Salir" << std::endl;
        std::cout << "Ingrese su opción: ";
        std::cin >> opcion;

        // Manejo de errores de entrada
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
            opcion = 0; 
        } else {
            switch (opcion) {
                case 1: {
                    std::cout << "Ha seleccionado la opción 1." << std::endl;

                    // Solicitar correo y contraseña
                    std::string correo;
                    std::string contrasena;

                    std::cout << "Ingrese su correo: ";
                    std::cin >> correo;

                    std::cout << "Ingrese su contraseña: ";
                    std::cin >> contrasena;

                    if (correo == admin_correo && contrasena == admin_contrasena) {
                        std::cout << "Bienvenido, admin." << std::endl;

                        int admin_opcion;
                        do {
                            std::cout << "\nMenu Administrador:\n";
                            std::cout << "1. Carga de usuarios\n";
                            std::cout << "2. Carga de relaciones\n";
                            std::cout << "3. Carga de publicaciones\n";
                            std::cout << "4. Gestionar usuarios\n";
                            std::cout << "5. Reportes\n";
                            std::cout << "6. Regresar al menú principal\n";
                            std::cout << "Seleccione una opción (0 para regresar al menú principal): ";
                            std::cin >> admin_opcion;

                            if (std::cin.fail()) {
                                std::cin.clear();
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
                                admin_opcion = -1; // Resetea la opción para evitar una selección inválida
                            } else {
                                // Mover la declaración de listaUsuarios aquí
                                ListaUsuarios listaUsuarios;

                                switch (admin_opcion) {
                                    case 1:
                                        std::cout << "Opción seleccionada: Carga de usuarios.\n";
                                        listaUsuarios = ListaUsuarios::cargarUsuariosDesdeJson("../usuarios.json");
                                        listaUsuarios.mostrarUsuarios();
                                        std::cout << "===============================\n" << std::endl;
                                        std::cout << "Usuarios cargados exitosamente.\n" << std::endl;
                                        std::cout << "===============================\n" << std::endl;
                                        break;
                                    case 2:
                                        std::cout << "Opción seleccionada: Carga de relaciones.\n";
                                        break;
                                    case 3:
                                        std::cout << "Opción seleccionada: Carga de publicaciones.\n";
                                        break;
                                    case 4:
                                        std::cout << "Opción seleccionada: Gestionar usuarios.\n";
                                        break;
                                    case 5:
                                        std::cout << "Opción seleccionada: Reportes.\n";
                                        break;
                                    case 6:
                                    case 0:
                                        std::cout << "Regresando al menú principal...\n";
                                        admin_opcion = 0; 
                                        break;
                                    default:
                                        std::cout << "Opción no válida. Por favor, intente nuevamente.\n";
                                        break;
                                }
                            }
                        } while (admin_opcion != 0);

                    } else {
                        std::cout << "Correo o contraseña incorrectos." << std::endl;
                    }
                    break;
                }
                case 2:{
                    std::cout << "Ha seleccionado la opción 2." << std::endl;

                    break;
                }
                case 3:{
                    std::cout <<"-----------------Informacion del estudiante-----------------" << std::endl;
                    std::cout << "Nombre: Kevin Andrés Alvarez Herrera" << std::endl;
                    std::cout << "Carnet: 202203038" << std::endl;
                    std::cout << "Ingenieria en ciencias y sistemas" << std::endl;
                    std::cout << "Curso: Estructura de datos" << std::endl;
                    std::cout << "Seccion: C" << std::endl;
                    std::cout << "----------------------------------" << std::endl;
                    break;
                }
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
