#ifndef GAME
#define GAME

#include <iostream>

struct GameRecord {
    int key;                        // Índice
    int Rank;                         // Rango
    char GameTitle[250];              // Título del juego
    char Platform[250];               // Plataforma
    int Year;                         // Año
    char Genre[250];                  // Género
    char Publisher[250];              // Editor
    double NorthAmerica;              // Ventas en Norteamérica
    double Europe;                    // Ventas en Europa
    double Japan;                     // Ventas en Japón
    double RestOfWorld;               // Ventas en el resto del mundo
    double Global;                    // Ventas globales
    double Review;                    // Reseña

    // Constructor por defecto
    GameRecord() : key(0), Rank(0), Year(0), NorthAmerica(0.0), Europe(0.0), 
                   Japan(0.0), RestOfWorld(0.0), Global(0.0), Review(0.0) {}
};

#endif // GAME

