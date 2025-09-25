#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// Constantes
const int MaxProductos = 100;
const int BajoStock = 10;

// Arreglos paralelos
int codigos[MaxProductos];
std::string nombres[MaxProductos];
int stock[MaxProductos];
float precios[MaxProductos];
std::string ubicaciones[MaxProductos];
int numProductos = 0;

//abrimos el archivo "inventario.txt"
void cargarInventario() {
    std::ifstream archivo("data/inventario.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir 'inventario.txt'. Se iniciara inventario vacio." << std::endl;
        return;
    }
    std::string linea;
    std::getline(archivo, linea); //Salta la primera linea
    numProductos = 0;
    while (std::getline(archivo, linea) && numProductos < MaxProductos) {
        std::stringstream ss(linea);
        std::string campo;
        // Código
        std::getline(ss, campo, ',');
        codigos[numProductos] = std::stoi(campo);
        // Nombre
        std::getline(ss, nombres[numProductos], ',');
        // Stock
        std::getline(ss, campo, ',');
        stock[numProductos] = std::stoi(campo);
        // Precio
        std::getline(ss, campo, ',');
        precios[numProductos] = std::stof(campo);
        // Ubicación
        std::getline(ss, ubicaciones[numProductos], ',');
        numProductos++;
    }
    archivo.close();
    std::cout << "Inventario cargado exitosamente. " << numProductos << " productos encontrados." << std::endl;
}

//Guardar inventario en archivo "inventario.txt"
void guardarInventario() {
    std::ofstream archivo("data/inventario.txt");
    archivo << "Código,Nombre,Cantidad,Precio,Ubicación\n"; //solucion pues mejor que se vuelva a escribir y me quito de problemas :)
    for (int i = 0; i < numProductos; ++i) {
        archivo << codigos[i] << "," << nombres[i] << "," << stock[i] << "," << precios[i] << "," << ubicaciones[i] << std::endl;
    }
    archivo.close();
    std::cout << "Cambios guardados en 'inventario.txt'." << std::endl;
}

//Funcion para solo aceptar numeros enteros 
void soloNumero(const std::string& mensaje, int& variable) {
    std::cout << mensaje;
    std::cin >> variable;
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Entrada invalida. Por favor ingrese un numero: ";
        std::cin >> variable;
    }
}

void mostrarMenu() {
    std::cout << "--- Menu principal ---" << std::endl;
    std::cout << "Seleccione una opcion:" << std::endl;
    std::cout << "1. Consultar un producto" << std::endl;
    std::cout << "2. Actualizar inventario por ubicacion" << std::endl;
    std::cout << "3. Registrar nuevo producto" << std::endl;
    std::cout << "4. Generar reporte de bajo stock" << std::endl;
    std::cout << "5. Encontrar el producto mas barato" << std::endl;
    std::cout << "6. Guardar y salir" << std::endl;
    std::cout << "Opcion seleccionada: ";
}

int main(){
        std::cout << "--- Bienvenido al Sistema de Inventario de \"El Martillo\" ---" << std::endl << std::endl;
        std::cout << "Cargando inventario desde 'inventario.txt'..." << std::endl;
        cargarInventario();
        int opcion = 0;

        do {
            mostrarMenu();
            soloNumero("", opcion);
            switch (opcion) {
                case 1: {
                    // Consultar un producto 
                    int codigo;
                    soloNumero("Ingrese el codigo del producto a consultar: ", codigo);
                    bool encontrado = false;
                    for (int i = 0; i < numProductos; ++i) {
                        if (codigos[i] == codigo) {
                            std::cout << "Nombre: " << nombres[i] << "\nStock: " << stock[i]
                                      << "\nPrecio: $" << precios[i] << "\nUbicacion: " << ubicaciones[i] << std::endl;
                            encontrado = true;
                            break;
                        }
                    }
                    if (!encontrado) std::cout << "Producto no encontrado." << std::endl;
                    break;
                }
                case 2: {
                    // Actualizar inventario por ubicacion
                    std::string ubicacion;
                    std::cout << "Ingrese la ubicacion del producto a actualizar: ";
                    std::cin >> ubicacion;
                    bool encontrado = false;
                    for (int i = 0; i < numProductos; ++i) {
                        if (ubicaciones[i] == ubicacion) {
                            int cantidad;
                            soloNumero("Ingrese la cantidad a agregar/quitar (+/-): ", cantidad);
                            if (stock[i] + cantidad < 0) {
                                std::cout << "Error: No se puede dejar el stock en negativo." << std::endl;
                            } else {
                                stock[i] += cantidad;
                                std::cout << "Cantidad actualizada exitosamente. El nuevo stock de \"" << nombres[i] << "\" es " << stock[i] << "." << std::endl;
                            }
                            encontrado = true;
                            break;
                        }
                    }
                    if (!encontrado) std::cout << "Ubicacion no encontrada." << std::endl;
                    break;
                }
                case 3: {
                    // Registrar nuevo producto
                    if (numProductos >= MaxProductos) {
                        std::cout << "Inventario lleno. No se pueden agregar mas productos." << std::endl;
                        break;
                    }
                    int codigo;
                    soloNumero("Ingrese el codigo del nuevo producto: ", codigo);
                    bool existe = false;
                    for (int i = 0; i < numProductos; ++i) {
                        if (codigos[i] == codigo) {
                            existe = true;
                            break;
                        }
                    }
                    if (existe) {
                        std::cout << "Error: El codigo ya existe." << std::endl;
                        break;
                    }
                    std::cin.ignore();
                    std::cout << "Ingrese el nombre del producto: ";
                    std::getline(std::cin, nombres[numProductos]);
                    soloNumero("Ingrese la cantidad en stock: ", stock[numProductos]);
                    std::cout << "Ingrese el precio unitario: ";
                    std::cin >> precios[numProductos];
                    std::cout << "Ingrese la ubicacion en almacen: ";
                    std::cin >> ubicaciones[numProductos];
                    codigos[numProductos] = codigo;
                    numProductos++;
                    std::cout << "Producto registrado exitosamente." << std::endl;
                    break;
                }
                case 4: {
                    // Generar reporte de bajo stock
                    std::cout << "--- Reporte de Productos con Bajo Stock ---" << std::endl;
                    for (int i = 0; i < numProductos; ++i) {
                        if (stock[i] < BajoStock) {
                            std::cout << "Nombre: " << nombres[i] << ", Stock: " << stock[i] << std::endl;
                        }
                    }
                    break;
                }
                case 5: {
                    // Encontrar el producto más barato
                    if (numProductos == 0) {
                        std::cout << "No hay productos en el inventario." << std::endl;
                        break;
                    }
                    int indiceMin = 0;
                    for (int i = 1; i < numProductos; ++i) {
                        if (precios[i] < precios[indiceMin]) {
                            indiceMin = i;
                        }
                    }
                    std::cout << "El producto mas barato es: " << nombres[indiceMin]
                              << " con un precio de $" << precios[indiceMin] << "." << std::endl;
                    break;
                }
                case 6: {
                    // Guardar y salir
                    std::cout << "Guardando cambios en 'inventario.txt'..." << std::endl;
                    guardarInventario();
                    std::cout << "Cambios guardados. Saliendo del sistema." << std::endl;
                    break;
                }
                default:
                    std::cout << "Opcion invalida. Intente de nuevo." << std::endl;
            }
        } while (opcion != 6);

        return 0;
    }
