# Sistema de Gestión de Biblioteca Escolar

## 📚 Descripción del Proyecto

Este sistema es una aplicación de consola desarrollada en C++ para la gestión de una base de datos relacional de una biblioteca escolar. El sistema permite realizar operaciones CRUD (Crear, Leer, Actualizar, Eliminar) sobre las entidades principales: **Autores**, **Libros**, **Estudiantes** y **Préstamos**.

La persistencia de los datos se maneja mediante archivos de texto (`.txt`) que simulan las tablas de una base de datos relacional normalizada hasta la Tercera Forma Normal (3FN).

## 💻 Requisitos de Ejecución

* **Sistema Operativo:** Windows con Subsistema de Windows para Linux (**WSL**) (Recomendado: Ubuntu).
* **Compilador C++:** **g++** (instalado por defecto en la mayoría de las distribuciones de WSL).
* **Editor de Código:** **Visual Studio Code (VS Code)** con la extensión **Remote - WSL**.

## ⚙️ Estructura de Archivos

| Archivo | Función |
| :--- | :--- |
| **`biblioteca.h`** | Contiene la definición de las **estructuras** de las entidades (Autor, Libro, Estudiante, Préstamo) y la declaración de la clase **`BibliotecaDB`**. |
| **`biblioteca.cpp`** | Contiene la implementación de todos los métodos (CRUD, Persistencia y Consultas) y la función **`main()`**. |
| **`autores.txt`** | Almacena los datos de la tabla `AUTOR`. |
| **`libros.txt`** | Almacena los datos de la tabla `LIBRO`. |
| **`estudiantes.txt`** | Almacena los datos de la tabla `ESTUDIANTE`. |
| **`prestamos.txt`** | Almacena los datos de la tabla `PRÉSTAMO`. |

## 🚀 Instrucciones de Compilación y Ejecución (WSL)

Para compilar y ejecutar el proyecto, sigue estos pasos en la terminal de WSL:

1.  **Navegar a la carpeta del proyecto** (Asegúrate de cambiar la ruta a la tuya):
    ```bash
    cd "/mnt/c/Users/hammy/OneDrive/Desktop/Key/Ciclo 2/Progra II"
    ```

2.  **Compilar el código fuente** (Esto genera el archivo ejecutable `biblioteca`):
    ```bash
    g++ biblioteca.cpp -o biblioteca
    ```

3.  **Ejecutar el programa** y acceder al menú principal:
    ```bash
    ./biblioteca
    ```
