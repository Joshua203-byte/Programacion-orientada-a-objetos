#include "biblioteca.h"

// ===========================================
//          IMPLEMENTACIÓN AUXILIAR
// ===========================================

void parsearLinea(const string& linea, vector<string>& campos) {
    campos.clear();
    stringstream ss(linea);
    string campo;
    while (getline(ss, campo, ',')) {
        campos.push_back(campo);
    }
}

template <typename T>
int obtenerNuevoId(const vector<T>& vec) {
    if (vec.empty()) {
        return 1;
    }
    auto max_it = max_element(vec.begin(), vec.end(), [](const T& a, const T& b) {
        return a.id < b.id;
    });
    return max_it->id + 1;
}

bool estaLibroPrestado(const vector<Prestamo>& prestamos, int id_libro) {
    return any_of(prestamos.begin(), prestamos.end(), [id_libro](const Prestamo& p){
        return p.id_libro == id_libro && (p.fecha_devolucion == "PENDIENTE" || p.fecha_devolucion.empty());
    });
}

// ===========================================
//              PERSISTENCIA
// ===========================================

void BibliotecaDB::guardarDatos() {
    // 1. Guardar Autores
    ofstream file_autores("autores.txt");
    for (const auto& a : autores) {
        file_autores << a.id << "," << a.nombre << "," << a.nacionalidad << endl;
    }
    file_autores.close();

    // 2. Guardar Libros
    ofstream file_libros("libros.txt");
    for (const auto& l : libros) {
        file_libros << l.id << "," << l.titulo << "," << l.isbn << ","
                    << l.ano_publicacion << "," << l.id_autor << endl;
    }
    file_libros.close();

    // 3. Guardar Estudiantes
    ofstream file_estudiantes("estudiantes.txt");
    for (const auto& e : estudiantes) {
        file_estudiantes << e.id << "," << e.nombre << "," << e.grado << endl;
    }
    file_estudiantes.close();

    // 4. Guardar Préstamos
    ofstream file_prestamos("prestamos.txt");
    for (const auto& p : prestamos) {
        file_prestamos << p.id << "," << p.id_libro << "," << p.id_estudiante << ","
                       << p.fecha_prestamo << "," << p.fecha_devolucion << endl;
    }
    file_prestamos.close();
    
    cout << "\n[Sistema] Datos guardados exitosamente.\n";
}

void BibliotecaDB::cargarDatos() {
    // 1. Cargar Autores
    ifstream file_autores("autores.txt");
    string linea;
    while (getline(file_autores, linea)) {
        vector<string> campos;
        parsearLinea(linea, campos);
        if (campos.size() == 3) {
            autores.push_back({stoi(campos[0]), campos[1], campos[2]});
        }
    }
    file_autores.close();

    // 2. Cargar Libros
    ifstream file_libros("libros.txt");
    while (getline(file_libros, linea)) {
        vector<string> campos;
        parsearLinea(linea, campos);
        if (campos.size() == 5) {
            libros.push_back({stoi(campos[0]), campos[1], campos[2], stoi(campos[3]), stoi(campos[4])});
        }
    }
    file_libros.close();
    
    // 3. Cargar Estudiantes
    ifstream file_estudiantes("estudiantes.txt");
    while (getline(file_estudiantes, linea)) {
        vector<string> campos;
        parsearLinea(linea, campos);
        if (campos.size() == 3) {
            estudiantes.push_back({stoi(campos[0]), campos[1], campos[2]});
        }
    }
    file_estudiantes.close();
    
    // 4. Cargar Préstamos
    ifstream file_prestamos("prestamos.txt");
    while (getline(file_prestamos, linea)) {
        vector<string> campos;
        parsearLinea(linea, campos);
        if (campos.size() == 5) {
            prestamos.push_back({stoi(campos[0]), stoi(campos[1]), stoi(campos[2]), campos[3], campos[4]});
        }
    }
    file_prestamos.close();

    cout << "[Sistema] Datos cargados: " << libros.size() << " libros, " << autores.size() << " autores, " << prestamos.size() << " préstamos." << endl;
}

// ===========================================
//                  CRUD
// ===========================================

// --- CRUD AUTOR ---

void BibliotecaDB::agregarAutor() {
    Autor a;
    a.id = obtenerNuevoId(autores);
    cout << "ID Autor asignado: " << a.id << endl;
    cout << "Nombre del autor: ";
    cin.ignore();
    getline(cin, a.nombre);
    cout << "Nacionalidad: ";
    getline(cin, a.nacionalidad);
    autores.push_back(a);
    cout << "[Éxito] Autor '" << a.nombre << "' agregado.\n";
}

void BibliotecaDB::listarAutores() const {
    if (autores.empty()) {
        cout << "[Info] No hay autores registrados.\n";
        return;
    }
    cout << "\n--- LISTA DE AUTORES ---\n";
    cout << left << setw(5) << "ID" << setw(30) << "NOMBRE" << setw(20) << "NACIONALIDAD" << endl;
    cout << string(55, '-') << endl;
    for (const auto& a : autores) {
        cout << left << setw(5) << a.id << setw(30) << a.nombre << setw(20) << a.nacionalidad << endl;
    }
}

void BibliotecaDB::eliminarAutor() {
    int id;
    cout << "ID del Autor a eliminar: ";
    cin >> id;

    // 1. Verificar FK (Manejo de Error: No eliminar autor con libros)
    if (any_of(libros.begin(), libros.end(), [id](const Libro& l){ return l.id_autor == id; })) {
        cout << "[Error] No se puede eliminar el autor. Hay libros asociados a este ID.\n";
        return;
    }

    // 2. Eliminar
    auto it = remove_if(autores.begin(), autores.end(), [id](const Autor& a){ return a.id == id; });
    if (it != autores.end()) {
        autores.erase(it, autores.end());
        cout << "[Éxito] Autor con ID " << id << " eliminado.\n";
    } else {
        cout << "[Error] Autor con ID " << id << " no encontrado.\n";
    }
}

// --- CRUD LIBRO ---

void BibliotecaDB::agregarLibro() {
    Libro l;
    l.id = obtenerNuevoId(libros);
    cout << "ID Libro asignado: " << l.id << endl;

    // Lógica para validar el ID de autor (FK Check)
    int id_a;
    cout << "ID del Autor (debe existir): ";
    cin >> id_a;
    auto it_autor = find_if(autores.begin(), autores.end(), [id_a](const Autor& a){ return a.id == id_a; });
    if (it_autor == autores.end()) {
        cout << "[Error] ID de Autor no válido. Libro no agregado.\n";
        return;
    }
    l.id_autor = id_a;

    cout << "Título: ";
    cin.ignore();
    getline(cin, l.titulo);
    cout << "ISBN: ";
    getline(cin, l.isbn);
    cout << "Año de Publicación: ";
    cin >> l.ano_publicacion;
    
    // 2. Validar ISBN Duplicado (Manejo de Error)
    if (any_of(libros.begin(), libros.end(), [&l](const Libro& lb){ return lb.isbn == l.isbn; })) {
        cout << "[Error] ISBN ya existe. Libro no agregado.\n";
        return;
    }

    libros.push_back(l);
    cout << "[Éxito] Libro '" << l.titulo << "' agregado.\n";
}

void BibliotecaDB::listarLibros() const {
    if (libros.empty()) {
        cout << "[Info] No hay libros registrados.\n";
        return;
    }
    cout << "\n--- LISTA DE LIBROS ---\n";
    cout << left << setw(5) << "ID" << setw(40) << "TÍTULO" << setw(15) << "ISBN" << setw(6) << "AÑO" << setw(10) << "ID AUTOR" << endl;
    cout << string(76, '-') << endl;
    for (const auto& l : libros) {
        cout << left << setw(5) << l.id << setw(40) << l.titulo << setw(15) << l.isbn << setw(6) << l.ano_publicacion << setw(10) << l.id_autor << endl;
    }
}

// --- CRUD ESTUDIANTE ---

void BibliotecaDB::agregarEstudiante() {
    Estudiante e;
    e.id = obtenerNuevoId(estudiantes);
    cout << "ID Estudiante asignado: " << e.id << endl;
    cout << "Nombre del estudiante: ";
    cin.ignore();
    getline(cin, e.nombre);
    cout << "Grado (ej. 2º Bachillerato): ";
    getline(cin, e.grado);
    estudiantes.push_back(e);
    cout << "[Éxito] Estudiante '" << e.nombre << "' agregado.\n";
}

void BibliotecaDB::listarEstudiantes() const {
    if (estudiantes.empty()) {
        cout << "[Info] No hay estudiantes registrados.\n";
        return;
    }
    cout << "\n--- LISTA DE ESTUDIANTES ---\n";
    cout << left << setw(5) << "ID" << setw(30) << "NOMBRE" << setw(20) << "GRADO" << endl;
    cout << string(55, '-') << endl;
    for (const auto& e : estudiantes) {
        cout << left << setw(5) << e.id << setw(30) << e.nombre << setw(20) << e.grado << endl;
    }
}


// ===========================================
//         CRUD PRÉSTAMO Y VALIDACIÓN
// ===========================================


void BibliotecaDB::realizarPrestamo() {
    Prestamo p;
    p.id = obtenerNuevoId(prestamos);
    cout << "ID Préstamo asignado: " << p.id << endl;

    // 1. Validar ID Libro
    cout << "ID del Libro a prestar: ";
    cin >> p.id_libro;
    auto it_libro = find_if(libros.begin(), libros.end(), [id=p.id_libro](const Libro& l){ return l.id == id; });
    if (it_libro == libros.end()) {
        cout << "[Error] Libro no encontrado.\n";
        return;
    }

    // 2. Manejo de Error: Préstamo de libro no disponible
    if (estaLibroPrestado(prestamos, p.id_libro)) {
        cout << "[Error] El libro '" << it_libro->titulo << "' ya se encuentra prestado.\n";
        return;
    }

    // 3. Validar ID Estudiante
    cout << "ID del Estudiante: ";
    cin >> p.id_estudiante;
    if (find_if(estudiantes.begin(), estudiantes.end(), [id=p.id_estudiante](const Estudiante& e){ return e.id == id; }) == estudiantes.end()) {
        cout << "[Error] Estudiante no encontrado.\n";
        return;
    }

    // 4. Asignar Fechas
    cout << "Fecha de Préstamo (YYYY-MM-DD): ";
    cin >> p.fecha_prestamo;
    p.fecha_devolucion = "PENDIENTE"; // Marcar como no devuelto

    prestamos.push_back(p);
    cout << "[Éxito] Préstamo registrado. Libro: '" << it_libro->titulo << "'.\n";
}

void BibliotecaDB::registrarDevolucion() {
    int id_libro;
    cout << "ID del Libro a devolver: ";
    cin >> id_libro;

    // Buscar el préstamo activo (sin fecha de devolución)
    auto it_prestamo = find_if(prestamos.begin(), prestamos.end(), [id_libro](Prestamo& p){
        return p.id_libro == id_libro && (p.fecha_devolucion == "PENDIENTE" || p.fecha_devolucion.empty());
    });

    if (it_prestamo == prestamos.end()) {
        cout << "[Error] No se encontró un préstamo activo para el ID de Libro " << id_libro << ".\n";
        return;
    }

    // Registrar la fecha de devolución
    string fecha;
    cout << "Fecha de Devolución (YYYY-MM-DD): ";
    cin >> fecha;
    it_prestamo->fecha_devolucion = fecha;

    cout << "[Éxito] Devolución registrada para el libro con ID " << id_libro << ".\n";
}

void BibliotecaDB::listarPrestamos() const {
    if (prestamos.empty()) {
        cout << "[Info] No hay préstamos registrados.\n";
        return;
    }
    cout << "\n--- HISTORIAL DE PRÉSTAMOS ---\n";
    cout << left << setw(5) << "ID" << setw(8) << "Libro" << setw(12) << "Estudiante" 
         << setw(15) << "F. PRÉSTAMO" << setw(15) << "F. DEVOL." << endl;
    cout << string(55, '-') << endl;
    for (const auto& p : prestamos) {
        cout << left << setw(5) << p.id << setw(8) << p.id_libro << setw(12) << p.id_estudiante
             << setw(15) << p.fecha_prestamo << setw(15) << p.fecha_devolucion << endl;
    }
}


// ===========================================
//                  CONSULTAS
// ===========================================

void BibliotecaDB::buscarLibrosPrestadosPorEstudiante() {
    int id_est;
    cout << "ID del Estudiante para buscar préstamos: ";
    cin >> id_est;

    auto it_est = find_if(estudiantes.begin(), estudiantes.end(), [id_est](const Estudiante& e){ return e.id == id_est; });
    if (it_est == estudiantes.end()) {
        cout << "[Error] Estudiante con ID " << id_est << " no encontrado.\n";
        return;
    }

    cout << "\n--- LIBROS PRESTADOS POR " << it_est->nombre << " ---\n";
    
    // Simulación de JOIN entre PRÉSTAMO y LIBRO
    for (const auto& p : prestamos) {
        if (p.id_estudiante == id_est) {
            auto it_libro = find_if(libros.begin(), libros.end(), [id_lib=p.id_libro](const Libro& l){ return l.id == id_lib; });
            
            if (it_libro != libros.end()) {
                string estado = (p.fecha_devolucion == "PENDIENTE" || p.fecha_devolucion.empty()) ? "(ACTIVO)" : "(DEVUELTO)";
                cout << "  - [" << estado << "] " << it_libro->titulo << " (ISBN: " << it_libro->isbn << ")" 
                     << " F. Prest.: " << p.fecha_prestamo << endl;
            }
        }
    }
}

void BibliotecaDB::autoresConMasLibros() const {
    map<int, int> conteo_libros; 

    for (const auto& l : libros) {
        conteo_libros[l.id_autor]++;
    }

    if (conteo_libros.empty()) {
        cout << "[Info] No hay libros para contar.\n";
        return;
    }

    vector<pair<int, int>> lista_autores(conteo_libros.begin(), conteo_libros.end());
    
    sort(lista_autores.begin(), lista_autores.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;
    });

    cout << "\n--- AUTORES CON MÁS LIBROS ---\n";
    int rank = 1;
    for (const auto& par : lista_autores) {
        auto it_autor = find_if(autores.begin(), autores.end(), [id=par.first](const Autor& a){ return a.id == id; });
        
        string nombre_autor = (it_autor != autores.end()) ? it_autor->nombre : "Desconocido";

        cout << rank++ << ". " << nombre_autor << " (ID: " << par.first << ") - " << par.second << " Libros\n";
        if (rank > 5) break; 
    }
}


// ===========================================
//               MENÚ PRINCIPAL
// ===========================================

void mostrarMenuPrincipal() {
    cout << "\n\n=== SISTEMA DE GESTIÓN DE BIBLIOTECA ===\n";
    cout << "1. Gestión de Libros\n";
    cout << "2. Gestión de Autores\n";
    cout << "3. Gestión de Estudiantes\n";
    cout << "4. Gestión de Préstamos/Devoluciones\n";
    cout << "5. Consultas e Informes\n";
    cout << "0. Salir (Guardar y Salir)\n";
    cout << "Ingrese opción: ";
}

void mostrarMenu(const string& titulo, const vector<string>& opciones) {
    cout << "\n--- " << titulo << " ---\n";
    for (size_t i = 0; i < opciones.size(); ++i) {
        cout << i + 1 << ". " << opciones[i] << endl;
    }
    cout << "0. Volver al Menú Principal\n";
    cout << "Ingrese opción: ";
}

int main() {
    // Inicializa la DB y carga los datos existentes
    BibliotecaDB db;
    int opcion_principal;
    int opcion_sub;

    do {
        mostrarMenuPrincipal();
        cin >> opcion_principal;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion_principal = -1;
            continue;
        }

        switch (opcion_principal) {
            case 1: // Gestión de Libros
                do {
                    mostrarMenu("GESTIÓN DE LIBROS", {"Agregar Libro", "Listar Libros"});
                    cin >> opcion_sub;
                    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); opcion_sub = -1; continue; }
                    switch (opcion_sub) {
                        case 1: db.agregarLibro(); break;
                        case 2: db.listarLibros(); break;
                    }
                } while (opcion_sub != 0);
                break;
            
            case 2: // Gestión de Autores
                do {
                    mostrarMenu("GESTIÓN DE AUTORES", {"Agregar Autor", "Listar Autores", "Eliminar Autor"});
                    cin >> opcion_sub;
                    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); opcion_sub = -1; continue; }
                    switch (opcion_sub) {
                        case 1: db.agregarAutor(); break;
                        case 2: db.listarAutores(); break;
                        case 3: db.eliminarAutor(); break; 
                    }
                } while (opcion_sub != 0);
                break;
                
            case 3: // Gestión de Estudiantes
                 do {
                    mostrarMenu("GESTIÓN DE ESTUDIANTES", {"Agregar Estudiante", "Listar Estudiantes"});
                    cin >> opcion_sub;
                    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); opcion_sub = -1; continue; }
                    switch (opcion_sub) {
                        case 1: db.agregarEstudiante(); break;
                        case 2: db.listarEstudiantes(); break;
                    }
                } while (opcion_sub != 0);
                break;

            case 4: // Gestión de Préstamos
                do {
                    mostrarMenu("GESTIÓN DE PRÉSTAMOS", {"Realizar Préstamo", "Registrar Devolución", "Listar Préstamos"});
                    cin >> opcion_sub;
                    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); opcion_sub = -1; continue; }
                    switch (opcion_sub) {
                        case 1: db.realizarPrestamo(); break; 
                        case 2: db.registrarDevolucion(); break;
                        case 3: db.listarPrestamos(); break;
                    }
                } while (opcion_sub != 0);
                break;

            case 5: // Consultas
                do {
                    mostrarMenu("CONSULTAS E INFORMES", {"Listar libros prestados por un estudiante", "Autores con más libros"});
                    cin >> opcion_sub;
                    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); opcion_sub = -1; continue; }
                    switch (opcion_sub) {
                        case 1: db.buscarLibrosPrestadosPorEstudiante(); break; 
                        case 2: db.autoresConMasLibros(); break; 
                    }
                } while (opcion_sub != 0);
                break;

            case 0:
                cout << "[Sistema] Saliendo del programa...\n";
                break;
            default:
                cout << "[Error] Opción no válida. Intente de nuevo.\n";
        }
    } while (opcion_principal != 0);

    return 0;
}