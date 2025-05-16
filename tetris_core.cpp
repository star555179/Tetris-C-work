#include "tetris_core.h"
#include <algorithm>

// 方块形状数据
const std::vector<std::vector<std::vector<std::pair<int, int>>>> tetrominoData = {
    // I形状
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}},
     {{1, 0}, {1, 1}, {1, 2}, {1, 3}}},
     // O形状
     {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
     // T形状
     {{{0, 0}, {1, 0}, {2, 0}, {1, 1}},
      {{1, 0}, {1, 1}, {1, 2}, {0, 1}},
      {{0, 1}, {1, 1}, {2, 1}, {1, 0}},
      {{1, 0}, {1, 1}, {1, 2}, {2, 1}}},
      // L形状
      {{{0, 0}, {1, 0}, {2, 0}, {2, 1}},
       {{1, 0}, {1, 1}, {1, 2}, {0, 2}},
       {{0, 1}, {1, 1}, {2, 1}, {0, 0}},
       {{1, 0}, {1, 1}, {1, 2}, {2, 0}}},
       // J形状
       {{{0, 0}, {1, 0}, {2, 0}, {0, 1}},
        {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
        {{0, 1}, {1, 1}, {2, 1}, {2, 0}},
        {{1, 0}, {1, 1}, {1, 2}, {0, 0}}},
        // S形状
        {{{1, 0}, {2, 0}, {0, 1}, {1, 1}},
         {{0, 0}, {0, 1}, {1, 1}, {1, 2}}},
         // Z形状
         {{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
          {{1, 0}, {1, 1}, {0, 1}, {0, 2}}}
};

Tetromino::Tetromino() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 6);

    shape = static_cast<TetrominoShape>(dis(gen));
    color = static_cast<TetrominoColor>(static_cast<int>(shape));
    rotationState = 0;

    int index = static_cast<int>(shape);
    const auto& shapeData = tetrominoData[index];
    const auto& rotation = shapeData[rotationState];

    for (const auto& pos : rotation) {
        blocks.push_back({ pos.first + 3, pos.second, color });
    }
}

void Tetromino::rotate() {
    int index = static_cast<int>(shape);
    const auto& shapeData = tetrominoData[index];
    int newRotation = (rotationState + 1) % shapeData.size();

    std::vector<Block> newBlocks = blocks;
    const auto& rotation = shapeData[newRotation];

    for (size_t i = 0; i < rotation.size(); ++i) {
        newBlocks[i].x = blocks[0].x + rotation[i].first - tetrominoData[index][rotationState][0].first;
        newBlocks[i].y = blocks[0].y + rotation[i].second - tetrominoData[index][rotationState][0].second;
    }

    blocks = newBlocks;
    rotationState = newRotation;
}

void Tetromino::move(int dx, int dy) {
    for (auto& block : blocks) {
        block.x += dx;
        block.y += dy;
    }
}

Playfield::Playfield(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<TetrominoColor>(width, TetrominoColor::Black));
}

bool Playfield::isPositionValid(const Tetromino& tetromino) const {
    for (const auto& block : tetromino.blocks) {
        if (block.x < 0 || block.x >= width || block.y >= height) {
            return false;
        }
        if (block.y >= 0 && grid[block.y][block.x] != TetrominoColor::Black) {
            return false;
        }
    }
    return true;
}

void Playfield::mergeTetromino(const Tetromino& tetromino) {
    for (const auto& block : tetromino.blocks) {
        if (block.y >= 0) {
            grid[block.y][block.x] = block.color;
        }
    }
}

int Playfield::clearLines() {
    int linesCleared = 0;
    for (int y = height - 1; y >= 0; --y) {
        bool isLineComplete = true;
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == TetrominoColor::Black) {
                isLineComplete = false;
                break;
            }
        }

        if (isLineComplete) {
            ++linesCleared;
            for (int y2 = y; y2 > 0; --y2) {
                grid[y2] = grid[y2 - 1];
            }
            grid[0] = std::vector<TetrominoColor>(width, TetrominoColor::Black);
            ++y; // 重新检查当前行（现在是上一行下移的）
        }
    }
    return linesCleared;
}

bool Playfield::isGameOver() const {
    for (int x = 0; x < width; ++x) {
        if (grid[0][x] != TetrominoColor::Black) {
            return true;
        }
    }
    return false;
}

TetrominoColor Playfield::getCellColor(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return TetrominoColor::Black;
}