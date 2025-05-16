#pragma once
#include "tetris_core.h"
#include "tetris_renderer.h"
#include "tetris_input.h"

// 游戏管理器类
class TetrisGame {
private:
    Playfield playfield;
    Tetromino currentTetromino;
    Tetromino nextTetromino;
    TetrisRenderer renderer;
    InputHandler inputHandler;
    int score;
    bool gameOver;
    bool isPaused;
    int dropCounter;
    int dropInterval;

public:
    TetrisGame();
    void initialize();
    void run();
    void processInput(GameAction action);
    void update();
    void render();
    void resetGame();
};    #pragma once
