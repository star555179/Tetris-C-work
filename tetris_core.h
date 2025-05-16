#pragma once
#include <vector>
#include <random>

// ������״����
enum class TetrominoShape {
    I, O, T, L, J, S, Z, None
};

// ������ɫ����
enum class TetrominoColor {
    Cyan, Yellow, Purple, Orange, Blue, Green, Red, Black
};

// ����������
struct Block {
    int x, y;
    TetrominoColor color;
};

// ����˹������
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

// ��Ϸ������
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
