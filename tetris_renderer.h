#pragma once
#include "tetris_core.h"
#include <windows.h>

// ‰÷»æ∆˜¿‡
class TetrisRenderer {
private:
    HANDLE consoleHandle;
    HWND consoleWindow;
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    CHAR_INFO* screenBuffer;
    int screenWidth, screenHeight;
    int playfieldOffsetX, playfieldOffsetY;
    int cellSize;

public:
    TetrisRenderer(int screenW, int screenH, int cellSz);
    ~TetrisRenderer();
    void initialize();
    void render(const Playfield& playfield, const Tetromino& currentTetromino,
        const Tetromino& nextTetromino, int score, bool gameOver);
    void clearScreen();
    void displayText(int x, int y, const char* text, WORD color = 0x0F);
};    #pragma once
