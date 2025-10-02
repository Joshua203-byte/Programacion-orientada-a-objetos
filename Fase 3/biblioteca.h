#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream> 
#include <algorithm> 
#include <map>       
#include <iomanip>
#include <limits> // ESTA ES LA LÍNEA AGREGADA PARA SOLUCIONAR EL ERROR DE COMPILACIÓN

using namespace std;

// === ESTRUCTURAS (Entidades de la BD) ===

// 1. Tabla AUTOR
struct Autor {
    int id;
    string nombre;
    string nacionalidad;
};

// 2. Tabla LIBRO
struct Libro {
    int id; // PK
    string titulo;
    string isbn;
    int ano_publicacion;
    int id_autor; // FK a Autor
};

// 3. Tabla ESTUDIANTE
struct Estudiante {
    int id;
    string nombre;
    string grado;
};

// 4. Tabla PRÉSTAMO
struct Prestamo {
    int id;
    int id_libro; // FK a Libro
    int id_estudiante; // FK a Estudiante
    string fecha_prestamo; // Formato YYYY-MM-DD
    string fecha_devolucion; // Opcional, "PENDIENTE" si no se ha devuelto
};


// === CLASE DE GESTIÓN DE LA BASE DE DATOS ===

class BibliotecaDB {
public:
    // Vectores para almacenar los datos en memoria (Tablas)
    vector<Autor> autores;
    vector<Libro> libros;
    vector<Estudiante> estudiantes;
    vector<Prestamo> prestamos;

    // CONSTRUCTOR: Carga datos al iniciar
    BibliotecaDB() {
        cargarDatos();
    }
    // DESTRUCTOR: Guarda datos al cerrar
    ~BibliotecaDB() {
        guardarDatos();
    }

    // --- PERSISTENCIA (Carga/Guardado) ---
    void cargarDatos();
    void guardarDatos();

    // --- CRUD Y CONSULTAS ---
    void agregarAutor();
    void listarAutores() const;
    void eliminarAutor();

    void agregarLibro();
    void listarLibros() const;

    void agregarEstudiante();
    void listarEstudiantes() const;

    void realizarPrestamo();
    void registrarDevolucion();
    void listarPrestamos() const;
    
    void buscarLibrosPrestadosPorEstudiante(); 
    void autoresConMasLibros() const; 
};

// Función auxiliar
template <typename T>
int obtenerNuevoId(const vector<T>& vec);
void parsearLinea(const string& linea, vector<string>& campos);
bool estaLibroPrestado(const vector<Prestamo>& prestamos, int id_libro);