#ifndef GAME_AVL
#define GAME_AVL

#include "game.h"

class GameRecordAVL : public GameRecord {
public:
    long left;                        // Puntero al hijo izquierdo
    long right;                       // Puntero al hijo derecho
    int height;                       // Altura del nodo

    // Constructor por defecto
    GameRecordAVL() : GameRecord(), left(-1), right(-1), height(0) {}
};

#endif // GAME_AVL

