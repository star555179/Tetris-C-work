#pragma once
#include <vector>
#include <random>

// 方块形状定义
enum class TetrominoShape {
    I, O, T, L, J, S, Z, None
};

// 方块颜色定义
enum class TetrominoColor {
    Cyan, Yellow, Purple, Orange, Blue, Green, Red, Black
};

// 单个方块类
struct Block {
    int x, y;
    TetrominoColor color;
};

// 俄罗斯方块类
class Tetromino {
public:
    TetrominoShape shape;
    TetrominoColor color;
    std::vector<Block> blocks;
    int rotationState;

    Tetromino();
    void rotate();
    void move(int dx, int dy);
};

// 游戏区域类
class Playfield {
private:
    int width, height;
    std::vector<std::vector<TetrominoColor>> grid;

public:
    Playfield(int w, int h);
    bool isPositionValid(const Tetromino& tetromino) const;
    void mergeTetromino(const Tetromino& tetromino);
    int clearLines();
    bool isGameOver() const;
    TetrominoColor getCellColor(int x, int y) const;
};    #pragma once
