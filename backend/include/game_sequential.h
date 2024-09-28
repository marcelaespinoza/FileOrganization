#ifndef GAME_SEQUENTIAL
#define GAME_SEQUENTIAL

#include "game.h"

class GameRecordSequential : public GameRecord {
public:
    int nextRecord;                   // Indica el siguiente registro
    char nextFileChar;                // Indica el archivo de siguiente registro
    char accFile;                     // Archivo de acceso
    int pos;                          // Posición del registro

    // Constructor por defecto
    GameRecordSequential() : GameRecord(), nextRecord(-1), nextFileChar('D'), accFile('D'), pos(0) {}

    // Método para mostrar información del juego en formato secuencial
};

#endif // GAME_SEQUENTIAL

