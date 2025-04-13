#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>

// Constantes (Pueden ser modificadas)
#define ITERACIONES 1000
#define AUMENTO 10
#define GRADO_MAXIMO 1000
#define GRADO_MINIMO 10

using namespace std;
using namespace std::chrono;

// Funciones de evaluación
long double horner_evaluacion(long double polinomio[], int grado, int x);
long double estandar_evaluacion(long double polinomio[], int degree, int x);

// Funciones para la creación del polinomio
int aleatorizador_n10_10();
long double* hacer_polinomio(int degree);

// Matrices para guardar tiempos
int horner_tiempos[ITERACIONES][GRADO_MAXIMO+1];
int estandar_tiempos[ITERACIONES][GRADO_MAXIMO+1];

// Arreglos para guardar tiempos promedios
int promedio_horner[GRADO_MAXIMO/AUMENTO];
int promedio_estandar[GRADO_MAXIMO/AUMENTO];

int main() {
    srand(time(NULL));

    int x, columna = 0;
    long double total_horner = 0, total_estandar = 0, fx;

    for (int j = GRADO_MINIMO ; j <= GRADO_MAXIMO ; j+=AUMENTO) {
        for(int i = 0; i < ITERACIONES; i++) {

            // Inicializar polinomio
            long double* polinomio = hacer_polinomio(j);
            x = aleatorizador_n10_10();

            // Estandar
            auto iniciar_estandar = chrono::high_resolution_clock::now(); // Iniciamos el cronometro
            fx = estandar_evaluacion(polinomio, j, x);                                 // Realizamos la evaluación
            auto parar_estandar = chrono::high_resolution_clock::now();   // Paramos el cronometro
            duration<double> duracion_estandar = parar_estandar - iniciar_estandar;           // Calculamos la diferencia
            estandar_tiempos[i][j] = duracion_estandar.count() * 1000000000;                  // Guardamos el tiempo en la matriz (Nanosegundos)
            total_estandar += duracion_estandar.count() * 1000000000;                         // Sumamos el tiempo al total

            // Horner
            auto iniciar_horner = chrono::high_resolution_clock::now();   // Iniciamos el cronometro
            fx = horner_evaluacion(polinomio, j, x);                                    // Realizamos la evaluación
            auto parar_horner = chrono::high_resolution_clock::now();     // Paramos el cronometro
            duration<double> duracion_horner = parar_horner - iniciar_horner;                 // Calculamos la diferencia
            horner_tiempos[i][j] = duracion_horner.count() * 1000000000;                      // Guardamos el tiempo en la matriz (Nanosegundos)
            total_horner += duracion_horner.count() * 1000000000;                             // Sumamos el tiempo al total

            // Borramos el polinomio
            delete[] polinomio;

        }

        // Asignamos los promedios
        promedio_horner[columna] = total_horner / ITERACIONES;
        promedio_estandar[columna] = total_estandar / ITERACIONES;

        // Reiniciamos el total de los promedios para el siguiente grado
        total_estandar = 0;
        total_horner = 0;

        // Pasamos a la siguiente columna
        columna++;
    }

    // Crear archivos .csv con los datos de tiempo de las evaluaciones
    ofstream salida_estandar("evaluacion_estandar.csv");
    ofstream salida_horner("evaluacion_horner.csv");
    if (!salida_estandar.is_open() || !salida_horner.is_open()) {
        cerr << "Error: No se puedo abir el archivo." << endl;
        return 1;
    }
    salida_estandar << ",";
    salida_horner << ",";
    for (int i = GRADO_MINIMO; i <= GRADO_MAXIMO; i+=AUMENTO) {
        salida_estandar << "Grado " << i << ",";
        salida_horner << "Grado " << i << ",";
    }
    salida_estandar << endl;
    salida_horner << endl;
    for (int i = 0; i < ITERACIONES; i++) {
        salida_estandar << i + 1 << ",";
        salida_horner << i + 1 << ",";
        for (int j = GRADO_MINIMO; j <= GRADO_MAXIMO; j+=AUMENTO) {
            salida_estandar << estandar_tiempos[i][j] << ",";
            salida_horner << horner_tiempos[i][j] << ",";
        }
        salida_estandar << endl;
        salida_horner << endl;
    }
    salida_estandar << "Promedio:,";
    salida_horner << "Promedio:,";
    for (int i = 0; i < GRADO_MAXIMO/AUMENTO; i++) {
        salida_estandar << promedio_estandar[i] << ",";
        salida_horner << promedio_horner[i] << ",";
    }
    salida_estandar.close();
    salida_horner.close();

    return 0;
}

// Evaluación estandar (basada en pseudocodigo)
long double estandar_evaluacion(long double polinomio[], int degree, int x) {
    long double polyval = polinomio[0];
    long double term;

    for (int i = 1 ; i <= degree ; i++) {
        term = polinomio[i];
        for (int j = 1 ; j <= i ; j++) {
            term *= x;
        }
        polyval = polyval + term;
    }

    return polyval;
}

// Evaluación de horner (basada en pseudocodigo)
long double horner_evaluacion(long double polinomio[], int grado, int x) {
    long double polyval = polinomio[grado];
    for (int i = 1 ; i <= grado ; i++) {
        polyval = polyval * x + polinomio[grado-i];
    }
    return polyval;
}

// Funcion que genera número aleatorio entre -10 y 10
int aleatorizador_n10_10() {
    int aleatorio = -10 + rand() % 21;
    return aleatorio;
}

// Función que crea el polinomio con coeficientes aleatorios
long double* hacer_polinomio(int degree) {
    long double* polinomio = new long double[degree+1];

    for (int i = 0; i < degree+1; i++) {
        polinomio[i] = aleatorizador_n10_10();
    }
    return polinomio;
}