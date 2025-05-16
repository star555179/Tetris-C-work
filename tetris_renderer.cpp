#include "tetris_renderer.h"
#include <iostream>
#include <string>

// 控制台颜色映射
const WORD colorMap[] = {
    0x0C, // 红色
    0x0A, // 绿色
    0x09, // 蓝色
    0x0E, // 黄色
    0x0D, // 紫色
    0x0F, // 白色
    0x0B, // 青色
    0x00  // 黑色
};

TetrisRenderer::TetrisRenderer(int screenW, int screenH, int cellSz) :
    screenWidth(screenW), screenHeight(screenH), cellSize(cellSz) {
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    playfieldOffsetX = (screenWidth - 10 * cellSize) / 2;
    playfieldOffsetY = 2;
}

TetrisRenderer::~TetrisRenderer() {
    delete[] screenBuffer;
}

void TetrisRenderer::initialize() {
    // 设置控制台窗口
    consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

    // 设置控制台缓冲区大小
    COORD bufferSize = { static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) };
    SetConsoleScreenBufferSize(consoleHandle, bufferSize);

    // 设置窗口大小
    SMALL_RECT windowRect = { 0, 0, static_cast<SHORT>(screenWidth - 1), static_cast<SHORT>(screenHeight - 1) };
    SetConsoleWindowInfo(consoleHandle, TRUE, &windowRect);

    // 创建屏幕缓冲区
    screenBuffer = new CHAR_INFO[screenWidth * screenHeight];
    clearScreen();
}

void TetrisRenderer::clearScreen() {
    for (int y = 0; y < screenHeight; ++y) {
        for (int x = 0; x < screenWidth; ++x) {
            screenBuffer[y * screenWidth + x].Char.AsciiChar = ' ';
            screenBuffer[y * screenWidth + x].Attributes = 0x00;
        }
    }
}

void TetrisRenderer::render(const Playfield& playfield, const Tetromino& currentTetromino,
    const Tetromino& nextTetromino, int score, bool gameOver) {
    clearScreen();

    // 绘制游戏标题
    displayText((screenWidth - 12) / 2, 0, "TETRIS GAME", 0x0F);

    // 绘制游戏区域边框
    for (int y = 0; y <= playfield.getHeight(); ++y) {
        for (int x = 0; x <= playfield.getWidth(); ++x) {
            if (x == 0 || x == playfield.getWidth() || y == playfield.getHeight()) {
                int screenX = playfieldOffsetX + x * cellSize;
                int screenY = playfieldOffsetY + y * cellSize;
                for (int cy = 0; cy < cellSize; ++cy) {
                    for (int cx = 0; cx < cellSize; ++cx) {
                        if (screenX + cx < screenWidth && screenY + cy < screenHeight) {
                            screenBuffer[(screenY + cy) * screenWidth + (screenX + cx)].Char.AsciiChar = '#';
                            screenBuffer[(screenY + cy) * screenWidth + (screenX + cx)].Attributes = 0x07;
                        }
                    }
                }
            }
        }
    }

    // 绘制游戏区域内的方块
    for (int y = 0; y < playfield.getHeight(); ++y) {
        for (int x = 0; x < playfield.getWidth(); ++x) {
            TetrominoColor color = playfield.getCellColor(x, y);
            if (color != TetrominoColor::Black) {
                int screenX = playfieldOffsetX + (x + 1) * cellSize;
                int screenY = playfieldOffsetY + y * cellSize;
                WORD consoleColor = colorMap[static_cast<int>(color)];
                for (int cy = 0; cy < cellSize; ++cy) {
                    for (int cx = 0; cx < cellSize; ++cx) {
                        if (screenX + cx < screenWidth && screenY + cy < screenHeight) {
                            screenBuffer[(screenY + cy) * screenWidth + (screenX + cx)].Char.AsciiChar = ' ';
                            screenBuffer[(screenY + cy) * screenWidth + (screenX + cx)].Attributes = consoleColor;
                        }
                    }
                }
            }
        }
    }

    // 绘制当前方块
    for (const auto& block : currentTetromino.blocks) {
        int screenX = playfieldOffsetX + (block.x + 1) * cellSize;
        int screenY = playfieldOffsetY + block.y * cellSize;
        WORD consoleColor = colorMap[static_cast<int>(block.color)];
        for (int cy = 0; cy < cellSize; ++cy) {
            for (int cx = 0; cx < cellSize; ++cx) {
                if (screenX + cx < screenWidth && screenY + cy < screenHeight && block.y >= 0) {
                    screenBuffer[(screenY + cy) * screenWidth + (screenX + cx)].Char.AsciiChar = ' ';
                    screenBuffer[(screenY + cy) * screenWidth + (screenX + cx)].Attributes = consoleColor;
                }
            }
        }
    }

    // 绘制下一个方块预览
    int nextOffsetX = playfieldOffsetX + playfield.getWidth() * cellSize + 10;
    int nextOffsetY = playfieldOffsetY + 4;
    displayText(nextOffsetX, nextOffsetY - 2, "NEXT:", 0x0F);

    for (const auto& block : nextTetromino.blocks) {
        int relX = block.x - nextTetromino.blocks[0].x;
        int relY = block.y - nextTetromino.blocks[0].y;
        int screenX = nextOffsetX + relX * cellSize;
        int screenY = nextOffsetY + relY * cellSize;
        WORD consoleColor = colorMap[static_cast<int>(block.color)];
        for (int cy = 0; cy < cellSize; ++cy) {
            for (int cx = 0; cx < cellSize; ++cx) {
                if (screenX + cx < screenWidth && screenY + cy < screenHeight) {
                    screenBuffer[(screenY + cy) * screenWidth + (screenX + cx)].Char.AsciiChar = ' ';
                    screenBuffer[(screenY + cy) * screenWidth + (screenX + cx)].Attributes = consoleColor;
                }
            }
        }
    }

    // 绘制分数
    std::string scoreText = "SCORE: " + std::to_string(score);
    displayText(nextOffsetX, nextOffsetY + 8, scoreText.c_str(), 0x0F);

    // 绘制控制说明
    displayText(nextOffsetX, nextOffsetY + 12, "CONTROLS:", 0x0F);
    displayText(nextOffsetX, nextOffsetY + 14, "LEFT:  Left Arrow", 0x0F);
    displayText(nextOffsetX, nextOffsetY + 16, "RIGHT: Right Arrow", 0x0F);
    displayText(nextOffsetX, nextOffsetY + 18, "DOWN:  Down Arrow", 0x0F);
    displayText(nextOffsetX, nextOffsetY + 20, "ROTATE: Up Arrow", 0x0F);
    displayText(nextOffsetX, nextOffsetY + 22, "DROP:   Space", 0x0F);
    displayText(nextOffsetX, nextOffsetY + 24, "PAUSE:  P", 0x0F);

    // 游戏结束显示
    if (gameOver) {
        displayText((screenWidth - 10) / 2, playfieldOffsetY + playfield.getHeight() / 2, "GAME OVER", 0x0C);
        displayText((screenWidth - 14) / 2, playfieldOffsetY + playfield.getHeight() / 2 + 2, "PRESS R TO RESTART", 0x0F);
    }

    // 更新屏幕
    SMALL_RECT rect = { 0, 0, static_cast<SHORT>(screenWidth - 1), static_cast<SHORT>(screenHeight - 1) };
    WriteConsoleOutput(consoleHandle, screenBuffer, { static_cast<SHORT>(screenWidth), static_cast<SHORT>(screenHeight) }, { 0, 0 }, &rect);
}

void TetrisRenderer::displayText(int x, int y, const char* text, WORD color) {
    if (x < 0 || y < 0 || x >= screenWidth || y >= screenHeight) return;

    int len = 0;
    while (text[len] != '\0' && x + len < screenWidth) {
        screenBuffer[y * screenWidth + x + len].Char.AsciiChar = text[len];
        screenBuffer[y * screenWidth + x + len].Attributes = color;
        ++len;
    }
}